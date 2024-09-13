/*****************************************************************************
 * Copyright (c) 2014-2024 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#include "../../../drawing/Drawing.h"
#include "../../../interface/Viewport.h"
#include "../../../ride/RideData.h"
#include "../../../ride/TrackData.h"
#include "../../../ride/TrackPaint.h"
#include "../../../sprites.h"
#include "../../../world/Map.h"
#include "../../Paint.h"
#include "../../support/MetalSupports.h"
#include "../../tile_element/Segment.h"
#include "../../track/Segment.h"
#include "../../track/Support.h"

using namespace OpenRCT2;

static constexpr TunnelGroup kTunnelGroup = TunnelGroup::Standard;

namespace OpenRCT2::VekomaMineRC
{
    static void VekomaMineRCTrackFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_FLAT + 0)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_FLAT + 1)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_FLAT + 2)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_FLAT + 3)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT + 0)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT + 1)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
            }
        }
        PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    static void VekomaMineRCTrackStation(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        static constexpr uint32_t imageIds[4][3] = {
            { (SPR_G2_VEKOMA_MINE_TRACK_BRAKE + 0), (SPR_G2_VEKOMA_MINE_TRACK_BLOCK_BRAKE + 0), SPR_STATION_BASE_B_SW_NE },
            { (SPR_G2_VEKOMA_MINE_TRACK_BRAKE + 1), (SPR_G2_VEKOMA_MINE_TRACK_BLOCK_BRAKE + 1), SPR_STATION_BASE_B_NW_SE },
            { (SPR_G2_VEKOMA_MINE_TRACK_BRAKE + 0), (SPR_G2_VEKOMA_MINE_TRACK_BLOCK_BRAKE + 0), SPR_STATION_BASE_B_SW_NE },
            { (SPR_G2_VEKOMA_MINE_TRACK_BRAKE + 1), (SPR_G2_VEKOMA_MINE_TRACK_BLOCK_BRAKE + 1), SPR_STATION_BASE_B_NW_SE },
        };

        if (trackElement.GetTrackType() == TrackElemType::EndStation)
        {
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours.WithIndex(imageIds[direction][1]), { 0, 0, height },
                { { 0, 6, height + 3 }, { 32, 20, 1 } });
        }
        else
        {
            PaintAddImageAsParentRotated(
                session, direction, session.TrackColours.WithIndex(imageIds[direction][0]), { 0, 0, height },
                { { 0, 6, height + 3 }, { 32, 20, 1 } });
        }
        PaintAddImageAsParentRotated(
            session, direction, GetStationColourScheme(session, trackElement).WithIndex(imageIds[direction][2]),
            { 0, 0, height - 2 }, { { 0, 2, height }, { 32, 28, 2 } });
        DrawSupportsSideBySide(session, direction, height, session.SupportColours, supportType.metal);
        TrackPaintUtilDrawStation2(session, ride, direction, height, trackElement, 4, 7);
        TrackPaintUtilDrawStationTunnel(session, direction, height);
        PaintUtilSetSegmentSupportHeight(session, kSegmentsAll, 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    static void VekomaMineRCTrack25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE + 8)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE + 9)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE + 10)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE + 11)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE + 8)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE + 9)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE + 10)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE + 11)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void VekomaMineRCTrack60DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 12)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 13)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 14)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 15)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 32, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 12)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 13)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 14)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 15)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 32, height, session.SupportColours);
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 56, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 104);
    }

    static void VekomaMineRCTrackFlatTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE + 0)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE + 1)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE + 2)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE + 3)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE + 0)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE + 1)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE + 2)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE + 3)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void VekomaMineRCTrack25DegUpTo60DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 0)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 1)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 2)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 3)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 4)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 5)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 12, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 0)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 1)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 2)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 3)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 4)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 5)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 12, height, session.SupportColours);
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 24, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 72);
    }

    static void VekomaMineRCTrack60DegUpTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 6)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 7)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 8)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 9)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 10)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP + 11)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 20, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 6)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 7)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 8)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 9)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 10)),
                        { 0, 0, height }, { { 0, 27, height }, { 32, 1, 66 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP + 11)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 20, height, session.SupportColours);
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 24, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 72);
    }

    static void VekomaMineRCTrack25DegUpToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE + 4)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE + 5)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE + 6)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE + 7)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE + 4)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE + 5)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE + 6)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE + 7)),
                        { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                    break;
            }
            if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
            {
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void VekomaMineRCTrack25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrack25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrack60DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrack60DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackFlatTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrack25DegUpToFlat(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrack25DegDownTo60DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrack60DegUpTo25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrack60DegDownTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrack25DegUpTo60DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrack25DegDownToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackFlatTo25DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }
       
    static void VekomaMineRCTrackLeftQuarterTurn3(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE + 3)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE + 6)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE + 9)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE + 1)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE + 4)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE + 7)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE + 10)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE + 2)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE + 5)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE + 8)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE + 11)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackRightQuarterTurn3(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        VekomaMineRCTrackLeftQuarterTurn3(session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftQuarterTurn5(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 5)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 10)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 1)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 6)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 11)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 16)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 2)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 7)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 12)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 17)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 3)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 8)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 13)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 18)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 4)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 9)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 14)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE + 19)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackRightQuarterTurn5(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        VekomaMineRCTrackLeftQuarterTurn5(session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftEighthToDiag(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 4)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 8)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 1)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 5)),
                            { 0, 0, height }, { { 0, 0, height }, { 34, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 9)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 13)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 2)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 6)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 10)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 14)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 3)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 7)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 18, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 11)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 15)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackRightEighthToDiag(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 16)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 20)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 24)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 28)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 17)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 21)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 25)),
                            { 0, 0, height }, { { 0, 0, height }, { 34, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 29)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 18)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 22)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 26)),
                            { 0, 0, height }, { { 4, 4, height }, { 28, 28, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 30)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 19)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 23)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 27)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 18, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE + 31)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackLeftEighthToOrthogonal(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        VekomaMineRCTrackRightEighthToDiag(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightEighthToOrthogonal(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        VekomaMineRCTrackLeftEighthToDiag(session, ride, trackSequence, (direction + 3) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        constexpr ImageIndex images[2][kNumOrthogonalDirections] = {
            { SPR_G2_VEKOMA_MINE_TRACK_FLAT_DIAGONAL + 0, SPR_G2_VEKOMA_MINE_TRACK_FLAT_DIAGONAL + 1,
              SPR_G2_VEKOMA_MINE_TRACK_FLAT_DIAGONAL + 0, SPR_G2_VEKOMA_MINE_TRACK_FLAT_DIAGONAL + 1 },
            { SPR_G2_VEKOMA_MINE_LIFT_TRACK_FLAT_DIAGONAL + 0, SPR_G2_VEKOMA_MINE_LIFT_TRACK_FLAT_DIAGONAL + 1,
              SPR_G2_VEKOMA_MINE_LIFT_TRACK_FLAT_DIAGONAL + 2, SPR_G2_VEKOMA_MINE_LIFT_TRACK_FLAT_DIAGONAL + 3 },
        };

        TrackPaintUtilDiagTilesPaintExtra(
            session, 3, height, direction, trackSequence, images[trackElement.HasChain()], supportType.metal);
    }

    static void VekomaMineRCTrackDiagBrakes(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BRAKE_DIAGONAL + 1)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BRAKE_DIAGONAL + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BRAKE_DIAGONAL + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BRAKE_DIAGONAL + 1)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackDiagBlockBrakes(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BLOCK_BRAKE_DIAGONAL + 1)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BLOCK_BRAKE_DIAGONAL + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BLOCK_BRAKE_DIAGONAL + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BLOCK_BRAKE_DIAGONAL + 1)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackDiag25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 9, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 9, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 9, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 9, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 9, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 9, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 9, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 9, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void VekomaMineRCTrackDiag25DegUpToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 6, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 5)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 6, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 6, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 6, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 6, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 5)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 6, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 6, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 6, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void VekomaMineRCTrackDiagFlatTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height + 2,
                                session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 0, height + 2,
                                session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 2, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 2, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 2, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 2, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void VekomaMineRCTrackDiag25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 9)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 9)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void VekomaMineRCTrackDiagFlatTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 5)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 5)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                break;
        }

        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void VekomaMineRCTrackDiag25DegDownToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_GENTLE_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void VekomaMineRCTrackDiag60DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 38, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 9)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 36, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 38, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 36, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 38, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 9)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 36, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 38, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 36, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
        }
    }

    static void VekomaMineRCTrackDiag25DegUpTo60DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 16, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 16, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 16, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 16, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 16, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 16, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 16, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 16, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackDiag60DegUpTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 21, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 5)),
                                { -16, -16, height }, { { 0, 0, height }, { 16, 16, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 21, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 21, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 21, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 21, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 5)),
                                { -16, -16, height }, { { 0, 0, height }, { 16, 16, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 21, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 21, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 21, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackDiag60DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 9)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 9)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 10)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 8)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 24, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 28, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 24, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 28, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 24, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 11)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 28, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 24, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 28, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 104);
                break;
        }
    }

    static void VekomaMineRCTrackDiag25DegDownTo60DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 5)),
                                { -16, -16, height }, { { 0, 0, height }, { 16, 16, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 5)),
                                { -16, -16, height }, { { 0, 0, height }, { 16, 16, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 6)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 4)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 17, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 17, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 17, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 17, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 17, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 7)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 17, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 17, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 17, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackDiag60DegDownTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 1)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 2)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 0)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_STEEP_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_DIAGONAL + 3)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackFlatToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 0)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 1)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 2)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 3)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 4)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 5)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
        }
        PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    static void VekomaMineRCTrackFlatToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 6)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 7)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 8)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 9)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 10)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 11)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
        }
        PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    static void VekomaMineRCTrackLeftBankToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackFlatToRightBank(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightBankToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackFlatToLeftBank(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftBankTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 12)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 13)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 14)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 15)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 16)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 17)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void VekomaMineRCTrackRightBankTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 18)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 19)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 20)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 21)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 22)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 23)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void VekomaMineRCTrack25DegUpToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 24)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 25)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 26)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 27)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 28)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 29)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void VekomaMineRCTrack25DegUpToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 30)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 31)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 32)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 33)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 34)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 35)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void VekomaMineRCTrackLeftBankTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrack25DegUpToRightBank(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightBankTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrack25DegUpToLeftBank(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrack25DegDownToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackRightBankTo25DegUp(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrack25DegDownToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackLeftBankTo25DegUp(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 36)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 37)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 38)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION + 39)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
        }
        PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    static void VekomaMineRCTrackRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackLeftBank(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagFlatToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 4)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 1)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 3)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 2)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackDiagFlatToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 9)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 5)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 7)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 8)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 6)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackDiagLeftBankToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 6)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 7)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 8)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 5)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 9)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackDiagRightBankToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 2)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 3)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 1)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 4)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackDiagLeftBankTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 14)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 10)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 11)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 13)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 12)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void VekomaMineRCTrackDiagRightBankTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 19)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 15)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 17)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 18)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 16)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void VekomaMineRCTrackDiag25DegUpToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 24)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 20)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 21)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 23)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 22)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void VekomaMineRCTrackDiag25DegUpToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 29)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 25)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 27)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 28)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 26)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void VekomaMineRCTrackDiagLeftBankTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 26)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 27)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 28)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 25)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 29)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                break;
        }

        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void VekomaMineRCTrackDiagRightBankTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 22)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 23)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 20)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 21)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 24)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                break;
        }

        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void VekomaMineRCTrackDiag25DegDownToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 16)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 17)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 18)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 15)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 19)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void VekomaMineRCTrackDiag25DegDownToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 12)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 13)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 10)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 11)),
                            { -16, -16, height }, { { -16, -16, height + 35 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 14)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalBSupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void VekomaMineRCTrackDiagLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 33)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 30)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 32)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 31)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackDiagRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 31)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 32)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 30)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BANK_TRANSITION_DIAGONAL + 33)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackLeftQuarterTurn3Bank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 0)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 1)), { 0, 0, height },
                            { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 4)), { 0, 0, height },
                            { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 7)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 11)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 2)), { 0, 0, height },
                            { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 5)), { 0, 0, height },
                            { { 0, 0, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 8)), { 0, 0, height },
                            { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 12)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 3)), { 0, 0, height },
                            { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 6)), { 0, 0, height },
                            { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 9)), { 0, 0, height },
                            { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 10)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_CURVE_BANKED + 13)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackRightQuarterTurn3Bank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        VekomaMineRCTrackLeftQuarterTurn3Bank(
            session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackBankedLeftQuarterTurn5(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 1)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 6)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 11)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 17)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 1, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 2)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 7)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 32, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 12)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 18)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 3)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 8)),
                            { 0, 0, height }, { { 16, 16, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 13)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 19)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 4)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 9)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 32, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 14)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 20)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 5)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 10)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 15)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 16)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_MEDIUM_CURVE_BANKED + 21)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 1, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackBankedRightQuarterTurn5(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        VekomaMineRCTrackBankedLeftQuarterTurn5(
            session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftEighthBankToDiag(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 0)), { 0, 0, height },
                            { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 4)), { 0, 0, height },
                            { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 8)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 1)), { 0, 0, height },
                            { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 5)), { 0, 0, height },
                            { { 0, 0, height + 27 }, { 34, 16, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 9)), { 0, 0, height },
                            { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 13)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 2)), { 0, 0, height },
                            { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 6)), { 0, 0, height },
                            { { 16, 16, height + 27 }, { 16, 16, 0 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 10)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 14)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 3)), { 0, 0, height },
                            { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 7)), { 0, 0, height },
                            { { 0, 16, height + 27 }, { 16, 18, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 11)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 15)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackRightEighthBankToDiag(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 16)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 20)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 24)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 28)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 17)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 21)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 25)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 34, 16, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 29)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 18)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 22)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 26)),
                            { 0, 0, height }, { { 4, 4, height + 27 }, { 28, 28, 0 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 30)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 19)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 0, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 23)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 0, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 27)),
                            { 0, 0, height }, { { 0, 16, height + 27 }, { 16, 18, 0 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 0, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_CURVE_BANKED + 31)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 0, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackLeftEighthBankToOrthogonal(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        VekomaMineRCTrackRightEighthBankToDiag(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightEighthBankToOrthogonal(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        VekomaMineRCTrackLeftEighthBankToDiag(
            session, ride, trackSequence, (direction + 3) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftQuarterTurn3Tile25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 0)), { 0, 6, height },
                            { 32, 20, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 2)), { 0, 6, height },
                            { 32, 20, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 4)), { 0, 6, height },
                            { 32, 20, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 6)), { 0, 6, height },
                            { 32, 20, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 1)), { 6, 0, height },
                            { 20, 32, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 3)), { 6, 0, height },
                            { 20, 32, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 5)), { 6, 0, height },
                            { 20, 32, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 7)), { 6, 0, height },
                            { 20, 32, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackRightQuarterTurn3Tile25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 8)), { 0, 6, height },
                            { 32, 20, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 10)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 12)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 14)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 9)), { 6, 0, height },
                            { 20, 32, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 11)),
                            { 6, 0, height }, { 20, 32, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 13)),
                            { 6, 0, height }, { 20, 32, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 10, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE + 15)),
                            { 6, 0, height }, { 20, 32, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                }
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackLeftQuarterTurn3Tile25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        VekomaMineRCTrackRightQuarterTurn3Tile25DegUp(
            session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightQuarterTurn3Tile25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        VekomaMineRCTrackLeftQuarterTurn3Tile25DegUp(
            session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftQuarterTurn525DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 5)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 10)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 1)),
                            { 0, 0, height }, { 32, 16, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 6)),
                            { 0, 0, height }, { 32, 16, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 11)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 16)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 2)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 7)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 12)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 17)),
                            { 0, 0, height }, { 16, 16, 3 });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 3)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 8)),
                            { 0, 0, height }, { 16, 32, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 13)),
                            { 0, 0, height }, { 16, 32, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 18)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 4)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 9)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 14)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 19)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackRightQuarterTurn525DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 20)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 25)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 30)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 35)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 21)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 26)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 31)),
                            { 0, 0, height }, { 32, 16, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 36)),
                            { 0, 0, height }, { 32, 16, 3 });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 22)),
                            { 0, 0, height }, { 16, 16, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 27)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 32)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 37)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 23)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 28)),
                            { 0, 0, height }, { 16, 32, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 33)),
                            { 0, 0, height }, { 16, 32, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 38)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 24)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 29)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 34)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE + 39)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackLeftQuarterTurn525DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        VekomaMineRCTrackRightQuarterTurn525DegUp(
            session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightQuarterTurn525DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        VekomaMineRCTrackLeftQuarterTurn525DegUp(
            session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftQuarterTurn160DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 0)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 1)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 2)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 3)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 4)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 5)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 6)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 7)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
        }
        TrackPaintUtilLeftQuarterTurn1TileTunnel(
            session, kTunnelGroup, direction, height, -8, TunnelSubType::SlopeStart, +56, TunnelSubType::SlopeEnd);
        PaintUtilSetSegmentSupportHeight(session, kSegmentsAll, 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 104);
    }

    static void VekomaMineRCTrackRightQuarterTurn160DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 8)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 9)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 10)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 11)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 12)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 13)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 14)),
                    { 0, 0, height }, { { 2, 2, height }, { 28, 28, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_STEEP_SMALL_CURVE + 15)),
                    { 0, 0, height }, { { 2, 2, height + 99 }, { 28, 28, 1 } });
                break;
        }
        TrackPaintUtilRightQuarterTurn1TileTunnel(
            session, kTunnelGroup, direction, height, -8, TunnelSubType::SlopeStart, +56, TunnelSubType::SlopeEnd);
        PaintUtilSetSegmentSupportHeight(session, kSegmentsAll, 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 104);
    }

    static void VekomaMineRCTrackLeftQuarterTurn160DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackRightQuarterTurn160DegUp(
            session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightQuarterTurn160DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackLeftQuarterTurn160DegUp(
            session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    
    static void VekomaMineRCTrack25DegUpToLeftBanked25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 0)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 1)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 2)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 3)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 4)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void VekomaMineRCTrack25DegUpToRightBanked25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 5)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 6)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 7)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 8)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 9)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void VekomaMineRCTrackLeftBanked25DegUpTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 10)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 11)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 12)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 13)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 14)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void VekomaMineRCTrackRightBanked25DegUpTo25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 15)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 16)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 17)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 18)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 19)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void VekomaMineRCTrackLeftBanked25DegDownTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrack25DegUpToRightBanked25DegUp(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightBanked25DegDownTo25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrack25DegUpToLeftBanked25DegUp(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrack25DegDownToLeftBanked25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackRightBanked25DegUpTo25DegUp(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrack25DegDownToRightBanked25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackLeftBanked25DegUpTo25DegUp(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftBankedFlatToLeftBanked25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 20)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 21)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 22)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 23)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void VekomaMineRCTrackRightBankedFlatToRightBanked25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 24)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 25)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 26)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 27)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void VekomaMineRCTrackLeftBanked25DegUpToLeftBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 28)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 29)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 30)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 31)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void VekomaMineRCTrackRightBanked25DegUpToRightBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 32)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 33)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 34)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 35)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void VekomaMineRCTrackLeftBankedFlatToLeftBanked25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackRightBanked25DegUpToRightBankedFlat(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightBankedFlatToRightBanked25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackLeftBanked25DegUpToLeftBankedFlat(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftBanked25DegDownToLeftBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackRightBankedFlatToRightBanked25DegUp(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightBanked25DegDownToRightBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackLeftBankedFlatToLeftBanked25DegUp(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrack25DegUpLeftBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 36)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 37)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 38)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 39)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void VekomaMineRCTrack25DegUpRightBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 40)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 41)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 42)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 43)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 56);
    }

    static void VekomaMineRCTrack25DegDownLeftBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrack25DegUpRightBanked(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrack25DegDownRightBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrack25DegUpLeftBanked(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackFlatToLeftBanked25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 44)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 45)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 46)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 47)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 48)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void VekomaMineRCTrackFlatToRightBanked25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 49)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 50)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 51)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 52)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 53)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 48);
    }

    static void VekomaMineRCTrackLeftBanked25DegUpToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 54)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 55)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 56)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 57)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 58)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void VekomaMineRCTrackRightBanked25DegUpToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 59)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 60)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 61)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 62)),
                    { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SLOPE_BANK_TRANSITION + 63)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::FlatTo25Deg);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 40);
    }

    static void VekomaMineRCTrackFlatToLeftBanked25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackRightBanked25DegUpToFlat(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackFlatToRightBanked25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackLeftBanked25DegUpToFlat(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftBanked25DegDownToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackFlatToRightBanked25DegUp(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightBanked25DegDownToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackFlatToLeftBanked25DegUp(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftBankedQuarterTurn3Tile25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 0)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 2)),
                            { 0, 6, height }, { { 0, 27, height }, { 32, 1, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 4)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 6)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 1)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 3)),
                            { 6, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 5)),
                            { 6, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 7)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackRightBankedQuarterTurn3Tile25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 8)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 10)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 12)),
                            { 0, 6, height }, { { 0, 27, height }, { 32, 1, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 14)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 9)),
                            { 6, 0, height }, { 20, 32, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 11)),
                            { 6, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 13)),
                            { 6, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 10, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_SMALL_CURVE_BANKED + 15)),
                            { 6, 0, height }, { 20, 32, 3 });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                        break;
                }
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackLeftBankedQuarterTurn3Tile25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        VekomaMineRCTrackRightBankedQuarterTurn3Tile25DegUp(
            session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightBankedQuarterTurn3Tile25DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        VekomaMineRCTrackLeftBankedQuarterTurn3Tile25DegUp(
            session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftBankedQuarterTurn525DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 5)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 10)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 1)),
                            { 0, 0, height }, { 32, 16, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 6)),
                            { 0, 0, height }, { { 30, 30, height }, { 1, 1, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 11)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 16)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 2)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 7)),
                            { 0, 0, height }, { { 30, 30, height }, { 1, 1, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 12)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 17)),
                            { 0, 0, height }, { 16, 16, 3 });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 3)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 8)),
                            { 0, 0, height }, { { 30, 30, height }, { 1, 1, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 13)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 18)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 4)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 9)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 14)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 19)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackRightBankedQuarterTurn525DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 20)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 25)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 30)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 35)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 21)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 26)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 31)),
                            { 0, 0, height }, { { 30, 30, height }, { 1, 1, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 36)),
                            { 0, 0, height }, { 32, 16, 3 });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 22)),
                            { 0, 0, height }, { 16, 16, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 27)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 32)),
                            { 0, 0, height }, { { 30, 30, height }, { 1, 1, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 37)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 23)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 28)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 33)),
                            { 0, 0, height }, { { 30, 30, height }, { 1, 1, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 38)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 24)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 29)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 34)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 34 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_MEDIUM_CURVE_BANKED + 39)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackLeftBankedQuarterTurn525DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        VekomaMineRCTrackRightBankedQuarterTurn525DegUp(
            session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightBankedQuarterTurn525DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        VekomaMineRCTrackLeftBankedQuarterTurn525DegUp(
            session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackSBendLeft(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 4)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 3)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 7)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                DrawSBendLeftSupports(session, supportType.metal, trackSequence, direction, height, 0, 0);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 1)),
                            { 0, 0, height }, { 32, 26, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 5)),
                            { 0, 0, height }, { 32, 26, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 2)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 6)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        break;
                }
                DrawSBendLeftSupports(
                    session, supportType.metal, trackSequence, direction, direction == 1 ? height - 1 : height, 0, 1);
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 2)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 6)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 1)),
                            { 0, 0, height }, { 32, 26, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 5)),
                            { 0, 0, height }, { 32, 26, 3 });
                        break;
                }
                DrawSBendLeftSupports(
                    session, supportType.metal, trackSequence, direction, direction == 3 ? height - 1 : height, 0, 0);
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 3)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 7)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 4)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                DrawSBendLeftSupports(session, supportType.metal, trackSequence, direction, height, 0, 0);
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackSBendRight(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 8)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 11)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                DrawSBendRightSupports(session, supportType.metal, trackSequence, direction, height, 0, 0);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 9)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 13)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 10)),
                            { 0, 0, height }, { 32, 26, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 14)),
                            { 0, 0, height }, { 32, 26, 3 });
                        break;
                }
                DrawSBendRightSupports(session, supportType.metal, trackSequence, direction, height, 0, 0);
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 10)),
                            { 0, 0, height }, { 32, 26, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 14)),
                            { 0, 0, height }, { 32, 26, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 9)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 13)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 26, 3 } });
                        break;
                }
                DrawSBendRightSupports(session, supportType.metal, trackSequence, direction, height, 0, 0);
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 11)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 8)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_S_BEND + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                DrawSBendRightSupports(session, supportType.metal, trackSequence, direction, height, 0, 0);
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackLeftHalfBankedHelixUpSmall(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 1)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 4)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 7)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 11)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 2)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 5)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 8)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 12)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 3)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 6)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 9)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 10)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 13)),
                            { 0, 0, height }, { { 6, 0, height + 8 }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 11)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 0)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 1)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 4)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 7)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 5:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 12)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 2)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 5)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 8)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 7:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 13)),
                            { 0, 0, height }, { { 0, 6, height + 8 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 3)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 6)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 9)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 10)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackRightHalfBankedHelixUpSmall(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 14)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 17)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 21)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 24)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 25)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 15)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 18)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 22)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 26)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 16)),
                            { 0, 0, height }, { { 6, 0, height + 8 }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 19)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 20)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 23)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 27)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 17)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 21)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 24)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 25)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 14)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 2, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 5:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 18)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 22)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 26)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 15)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 7:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 19)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 20)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 23)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 27)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_HELIX + 16)),
                            { 0, 0, height }, { { 0, 6, height + 8 }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 6, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackLeftHalfBankedHelixDownSmall(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackSequence >= 4)
        {
            trackSequence -= 4;
            direction = (direction - 1) & 3;
        }
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        VekomaMineRCTrackRightHalfBankedHelixUpSmall(
            session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightHalfBankedHelixDownSmall(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackSequence >= 4)
        {
            trackSequence -= 4;
            direction = (direction + 1) & 3;
        }
        trackSequence = kMapLeftQuarterTurn3TilesToRightQuarterTurn3Tiles[trackSequence];
        VekomaMineRCTrackLeftHalfBankedHelixUpSmall(
            session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftHalfBankedHelixUpLarge(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 1)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 6)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 11)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 17)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 1, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 2)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 7)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 32, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 12)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 18)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 3)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 8)),
                            { 0, 0, height }, { { 16, 16, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 13)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 19)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 4)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 9)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 32, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 14)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 20)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 5)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 10)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 15)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 16)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 21)),
                            { 0, 0, height }, { { 6, 0, height + 8 }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height + 8, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height + 8, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 7:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 17)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 0)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 1)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 6)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 11)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 1, height, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 8:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 9:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 18)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 2)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 7)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 32, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 12)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 10:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 19)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 3)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 8)),
                            { 0, 0, height }, { { 16, 16, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 13)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 11:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 12:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 20)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 4)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 9)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 32, 16, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 14)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 13:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 21)),
                            { 0, 0, height }, { { 0, 6, height + 8 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 5)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 10)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 16)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }
    static void VekomaMineRCTrackRightHalfBankedHelixUpLarge(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 22)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 27)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 33)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 38)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 39)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 1, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 23)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 28)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 34)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 32, 16, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 40)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 24)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 29)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 35)),
                            { 0, 0, height }, { { 16, 16, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 41)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 4:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 5:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 25)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 30)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 36)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 32, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 42)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 6:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 26)),
                            { 0, 0, height }, { { 6, 0, height + 8 }, { 20, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 31)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 32)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 37)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 43)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                        break;
                }
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height + 8, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height + 8, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 7:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 27)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 33)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 38)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 39)),
                            { 0, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 22)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 1, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 8:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 9:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 28)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 34)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 16, 32, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 40)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 23)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 10:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 29)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 35)),
                            { 0, 0, height }, { { 16, 16, height + 27 }, { 16, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 41)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 24)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 11:
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 12:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 30)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 36)),
                            { 0, 0, height }, { { 0, 0, height + 27 }, { 32, 16, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 42)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 25)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
            case 13:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 31)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 32)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 37)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 43)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_LARGE_HELIX + 26)),
                            { 0, 0, height }, { { 0, 6, height + 8 }, { 32, 20, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::Centre, 7, height, session.SupportColours);
                        break;
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
                break;
        }
    }

    static void VekomaMineRCTrackLeftHalfBankedHelixDownLarge(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackSequence >= 7)
        {
            trackSequence -= 7;
            direction = (direction - 1) & 3;
        }
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        VekomaMineRCTrackRightHalfBankedHelixUpLarge(
            session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightHalfBankedHelixDownLarge(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackSequence >= 7)
        {
            trackSequence -= 7;
            direction = (direction + 1) & 3;
        }
        trackSequence = kMapLeftQuarterTurn5TilesToRightQuarterTurn5Tiles[trackSequence];
        VekomaMineRCTrackLeftHalfBankedHelixUpLarge(
            session, ride, trackSequence, (direction - 1) & 3, height, trackElement, supportType);
    }

    
    static void VekomaMineRCTrackBrakes(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BRAKE + 0)), { 0, 0, height },
                    { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BRAKE + 1)), { 0, 0, height },
                    { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
        }
        PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    static void VekomaMineRCTrackOnRidePhoto(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        TrackPaintUtilOnridePhotoPlatformPaint(session, direction, height, supportType.metal);

        switch (direction)
        {
            case 0:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT + 0)), { 0, 0, height },
                    { { 0, 6, height + 3 }, { 32, 20, 0 } });
                break;
            case 1:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT + 1)), { 0, 0, height },
                    { { 0, 6, height + 3 }, { 32, 20, 0 } });
                break;
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT + 0)), { 0, 0, height },
                    { { 0, 6, height + 3 }, { 32, 20, 0 } });
                break;
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT + 1)), { 0, 0, height },
                    { { 0, 6, height + 3 }, { 32, 20, 0 } });
                break;
        }
        TrackPaintUtilOnridePhotoPaint2(session, direction, trackElement, height);
    }

    static void VekomaMineRCTrackFlatTo60DegUpLongBase(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 0)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 4)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 8)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 1)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 5)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 9)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 13)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 9, height, session.SupportColours);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 2)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 6)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 10)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 14)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 10, height, session.SupportColours);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 3)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 7)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 11)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 15)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 19, height, session.SupportColours);
                }
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height + 24, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height + 24, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 80);
                break;
        }
    }
    static void VekomaMineRCTrack60DegUpToFlatLongBase(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 16)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 20)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 24)),
                            { 0, 0, height }, { { 0, 27, height }, { 32, 1, 98 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 28)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 24, height, session.SupportColours);
                }
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 80);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 17)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 21)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 25)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 29)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 18, height, session.SupportColours);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 80);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 18)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 22)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 26)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 30)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 13, height, session.SupportColours);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 19)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 23)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 27)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_FLAT_TO_STEEP + 31)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
                {
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 5, height, session.SupportColours);
                }
                switch (direction)
                {
                    case 1:
                        PaintUtilPushTunnelRight(session, height + 8, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 2:
                        PaintUtilPushTunnelLeft(session, height + 8, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 40);
                break;
        }
    }

    static void VekomaMineRCTrackFlatTo60DegDownLongBase(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrack60DegUpToFlatLongBase(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrack60DegDownToFlatLongBase(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackFlatTo60DegUpLongBase(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackBlockBrakes(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (direction)
        {
            case 0:
            case 2:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BLOCK_BRAKE + 0)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
            case 1:
            case 3:
                PaintAddImageAsParentRotated(
                    session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_BLOCK_BRAKE + 1)),
                    { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                break;
        }
        if (TrackPaintUtilShouldPaintSupports(session.MapPosition))
        {
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 0, height, session.SupportColours);
        }
        PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + kDefaultGeneralSupportHeight);
    }

    


    static void VekomaMineRCTrackLeftBankToLeftQuarterTurn3Tile25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 0)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 2)),
                            { 0, 6, height }, { 32, 20, 3 });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 3)),
                            { 0, 6, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 5)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 7)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 1)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 4)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 6)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 8)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height - 6, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
        }
    }

    static void VekomaMineRCTrackRightBankToRightQuarterTurn3Tile25DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 9)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 11)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 13)),
                            { 0, 6, height }, { 32, 20, 3 });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 14)),
                            { 0, 6, height }, { { 0, 27, height }, { 32, 1, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 16)),
                            { 0, 6, height }, { 32, 20, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 10)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 12)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 15)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 17)),
                            { 6, 0, height }, { { 0, 6, height - 6 }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height - 6, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::SlopeEnd);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
        }
    }

    static void VekomaMineRCTrackLeftQuarterTurn3Tile25DegDownToLeftBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 12)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 15)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 17)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 10)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height - 6, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::SlopeEnd);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 11)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 13)),
                            { 6, 0, height }, { 20, 32, 3 });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 14)),
                            { 6, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 16)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 9)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                switch (direction)
                {
                    case 2:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 3:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
        }
    }

    static void VekomaMineRCTrackRightQuarterTurn3Tile25DegDownToRightBank(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 8)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 1)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 4)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 6)),
                            { 0, 6, height }, { { 0, 6, height - 6 }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height - 6, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::SlopeEnd);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 1:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 7)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 0)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 2)),
                            { 6, 0, height }, { 20, 32, 3 });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 3)),
                            { 6, 0, height }, { { 27, 0, height }, { 1, 32, 26 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_TURN_BANK_TRANSITION + 5)),
                            { 6, 0, height }, { 20, 32, 3 });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                switch (direction)
                {
                    case 0:
                        PaintUtilPushTunnelRight(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                    case 1:
                        PaintUtilPushTunnelLeft(session, height, kTunnelGroup, TunnelSubType::Flat);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
        }
    }
       
    static void VekomaMineRCTrackFlatTo60DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 0)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 2)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 1)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 0, height + 4, session.SupportColours);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 4)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 3)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 0, height + 4, session.SupportColours);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 5)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                    break;
            }
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 0)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 2)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 1)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 0, height + 4, session.SupportColours);
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 4)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 3)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 0, height + 4, session.SupportColours);
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 5)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    MetalASupportsPaintSetup(
                        session, supportType.metal, MetalSupportPlace::Centre, 3, height, session.SupportColours);
                    break;
            }
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height, kTunnelGroup, TunnelSubType::Flat);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 24, kTunnelGroup, TunnelSubType::SlopeEnd);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 64);
    }

    static void VekomaMineRCTrack60DegUpToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        if (trackElement.HasChain())
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 6)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 7)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 8)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 10)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 9)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction,
                        session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 11)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    break;
            }
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 16, height, session.SupportColours);
        }
        else
        {
            switch (direction)
            {
                case 0:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 6)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    break;
                case 1:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 7)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 8)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    break;
                case 2:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 10)),
                        { 0, 0, height }, { { 29, 4, height + 2 }, { 1, 24, 43 } });
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 9)),
                        { 0, 0, height }, { { 0, 4, height }, { 32, 2, 43 } });
                    break;
                case 3:
                    PaintAddImageAsParentRotated(
                        session, direction, session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 11)),
                        { 0, 0, height }, { { 0, 2, height }, { 32, 27, 4 } });
                    break;
            }
            MetalASupportsPaintSetup(session, supportType.metal, MetalSupportPlace::Centre, 16, height, session.SupportColours);
        }
        if (direction == 0 || direction == 3)
        {
            PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
        }
        else
        {
            PaintUtilPushTunnelRotated(session, direction, height + 24, kTunnelGroup, TunnelSubType::Flat);
        }
        PaintUtilSetSegmentSupportHeight(
            session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
        PaintUtilSetGeneralSupportHeight(session, height + 72);
    }

    static void VekomaMineRCTrackFlatTo60DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrack60DegUpToFlat(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrack60DegDownToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackFlatTo60DegUp(session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagFlatTo60DegUp(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 15)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 15)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 12)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 12)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 14)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 14)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 7, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 13)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 7, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 7, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 7, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 7, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 13)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 7, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 7, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 7, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
        }
    }

    static void VekomaMineRCTrackDiag60DegUpToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 19)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 19)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 16)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 16)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 18)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 18)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 20, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 17)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 20, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 20, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 20, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 20, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 17)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 20, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 20, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 20, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackDiagFlatTo60DegDown(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 17)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 17)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 18)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 18)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 16)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 16)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 16, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 19)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 16, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 16, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 16, height,
                                session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 16, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 19)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 16, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 16, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 16, height,
                                session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackDiag60DegDownToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 13)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 3:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 13)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 1:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 14)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 14)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 2:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 12)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 2:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 12)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
            case 3:
                if (trackElement.HasChain())
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 5, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_LIFT_TRACK_SMALL_FLAT_TO_STEEP + 15)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 5, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 5, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 5, height, session.SupportColours);
                            break;
                    }
                }
                else
                {
                    switch (direction)
                    {
                        case 0:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::LeftCorner, 5, height, session.SupportColours);
                            break;
                        case 1:
                            PaintAddImageAsParentRotated(
                                session, direction,
                                session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_SMALL_FLAT_TO_STEEP + 15)),
                                { -16, -16, height }, { { -16, -16, height }, { 32, 32, 4 } });
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::TopCorner, 5, height, session.SupportColours);
                            break;
                        case 2:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::RightCorner, 5, height, session.SupportColours);
                            break;
                        case 3:
                            MetalBSupportsPaintSetup(
                                session, supportType.metal, MetalSupportPlace::BottomCorner, 5, height, session.SupportColours);
                            break;
                    }
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 64);
                break;
        }
    }

    // EnumsToFlags(PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::rightCorner, PaintSegment::bottomCorner,
    // PaintSegment::centre, PaintSegment::topLeftSide, PaintSegment::topRightSide, PaintSegment::bottomLeftSide,
    // PaintSegment::bottomRightSide);
    //     b4
    //   c8  cc
    // b8  c4  bc
    //   d0  d4
    //     c0

    //    0
    //  5   6
    // 1   4   2
    //  7   8
    //    3

    // 3102
    // 8756

    static void VekomaMineRCTrackLeftEighthToDiagUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 0)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 4)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 8)), { 0, 0, height },
                            { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 12)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 1)), { 0, 0, height },
                            { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 5)), { 0, 0, height },
                            { { 0, 0, height }, { 34, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 9)), { 0, 0, height },
                            { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 13)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 2)), { 0, 0, height },
                            { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 6)), { 0, 0, height },
                            { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 10)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 14)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 3)), { 0, 0, height },
                            { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 7)), { 0, 0, height },
                            { { 0, 16, height }, { 16, 18, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 11)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 15)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackRightEighthToDiagUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 16)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 20)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 24)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 28)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 17)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 21)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 25)),
                            { 0, 0, height }, { { 0, 0, height }, { 34, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 29)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 18)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 22)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 26)),
                            { 0, 0, height }, { { 4, 4, height }, { 28, 28, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 30)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 19)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 23)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 27)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 18, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 31)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackLeftEighthToOrthogonalUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 32)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 36)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 18, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 40)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 44)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 33)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 37)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 41)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 45)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 34)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 38)),
                            { 0, 0, height }, { { 0, 0, height }, { 34, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 42)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 46)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 35)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 39)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 43)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 47)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);

                if (direction == 1 || direction == 2)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                }

                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackRightEighthToOrthogonalUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 48)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 52)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 56)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 18, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 60)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 49)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 53)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 57)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 61)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 50)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 54)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 58)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 62)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 51)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 55)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 59)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE + 63)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 1)
                {
                    PaintUtilPushTunnelRotated(session, direction + 1, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(PaintSegment::centre, PaintSegment::topLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackLeftEighthToDiagDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        uint8_t map[5] = { 4, 3, 1, 2, 0 };
        trackSequence = map[trackSequence];
        VekomaMineRCTrackRightEighthToOrthogonalUp25(
            session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightEighthToDiagDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        uint8_t map[5] = { 4, 3, 1, 2, 0 };
        trackSequence = map[trackSequence];
        VekomaMineRCTrackLeftEighthToOrthogonalUp25(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftEighthToOrthogonalDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        VekomaMineRCTrackRightEighthToDiagUp25(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightEighthToOrthogonalDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        VekomaMineRCTrackLeftEighthToDiagUp25(
            session, ride, trackSequence, (direction + 3) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagUp25ToLeftBankedUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 4)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 0)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 1)),
                            { -16, -16, height }, { { -16, -16, height + 48 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 3)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 9, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 2)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 9, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 9, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 9, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackDiagUp25ToRightBankedUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 9)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 5)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 7)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 8)),
                            { -16, -16, height }, { { -16, -16, height + 48 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 9, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 6)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 9, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 9, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 9, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackDiagLeftBankedUp25ToUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 14)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 10)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 11)),
                            { -16, -16, height }, { { -16, -16, height + 48 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 13)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 9, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 12)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 9, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 9, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 9, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void VekomaMineRCTrackDiagRightBankedUp25ToUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 19)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 15)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 17)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 18)),
                            { -16, -16, height }, { { -16, -16, height + 48 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 9, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 16)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 9, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 9, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 9, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void VekomaMineRCTrackDiagDown25ToLeftBankedDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagRightBankedUp25ToUp25(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagDown25ToRightBankedDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagLeftBankedUp25ToUp25(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagLeftBankedDown25ToDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagUp25ToRightBankedUp25(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagRightBankedDown25ToDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagUp25ToLeftBankedUp25(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagLeftBankedFlatToLeftBankedUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 23)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 20)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 22)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 21)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void VekomaMineRCTrackDiagRightBankedFlatToRightBankedUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 27)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 24)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 26)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 25)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void VekomaMineRCTrackDiagLeftBankedUp25ToLeftBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 31)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 28)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 30)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 6, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 29)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 6, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 6, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 6, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void VekomaMineRCTrackDiagRightBankedUp25ToRightBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 35)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 32)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 34)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 6, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 33)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 6, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 6, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 6, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void VekomaMineRCTrackDiagLeftBankedFlatToLeftBankedDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagRightBankedUp25ToRightBankedFlat(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagRightBankedFlatToRightBankedDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagLeftBankedUp25ToLeftBankedFlat(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagLeftBankedDown25ToLeftBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagRightBankedFlatToRightBankedUp25(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagRightBankedDown25ToRightBankedFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagLeftBankedFlatToLeftBankedUp25(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagUp25LeftBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 39)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 36)),
                            { -16, -16, height }, { { -16, -16, height + 48 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 38)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 11, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 37)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 11, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 11, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 11, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void VekomaMineRCTrackDiagUp25RightBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 43)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 40)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 42)),
                            { -16, -16, height }, { { -16, -16, height + 48 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 11, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 41)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 11, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 11, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 11, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void VekomaMineRCTrackDiagDown25LeftBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagUp25RightBanked(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagDown25RightBanked(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagUp25LeftBanked(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagFlatToLeftBankedUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 48)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 44)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 45)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 47)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 46)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void VekomaMineRCTrackDiagFlatToRightBankedUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 53)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 49)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 51)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 52)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 4, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 50)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 4, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 4, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 4, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 48);
                break;
        }
    }

    static void VekomaMineRCTrackDiagLeftBankedUp25ToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 58)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 54)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 55)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 57)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 6, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 56)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 6, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 6, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 6, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void VekomaMineRCTrackDiagRightBankedUp25ToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 63)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topRightSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 59)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 2:
                switch (direction)
                {
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 61)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 62)),
                            { -16, -16, height }, { { -16, -16, height + 27 }, { 32, 32, 0 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 6, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 60)),
                            { -16, -16, height }, { { -16, -16, height }, { 32, 32, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 6, height, session.SupportColours);
                        break;
                    case 2:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 6, height, session.SupportColours);
                        break;
                    case 3:
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 6, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 56);
                break;
        }
    }

    static void VekomaMineRCTrackDiagFlatToLeftBankedDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagRightBankedUp25ToFlat(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagFlatToRightBankedDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagLeftBankedUp25ToFlat(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagLeftBankedDown25ToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagFlatToRightBankedUp25(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackDiagRightBankedDown25ToFlat(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        VekomaMineRCTrackDiagFlatToLeftBankedUp25(
            session, ride, 3 - trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftEighthBankToDiagUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 64)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 68)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 72)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 76)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 65)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 69)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 73)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 77)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 66)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 70)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 40, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 74)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 78)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 67)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 71)),
                            { 0, 0, height }, { { 0, 0, height + 48 }, { 32, 32, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 75)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 79)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackRightEighthBankToDiagUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 80)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 84)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 88)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 92)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 3)
                {
                    PaintUtilPushTunnelRotated(session, direction, height - 8, kTunnelGroup, TunnelSubType::SlopeStart);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 81)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 85)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 89)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 93)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 82)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 86)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 90)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 40, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 94)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 83)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 87)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 91)),
                            { 0, 0, height }, { { 0, 0, height + 48 }, { 32, 32, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 95)),
                            { 0, 0, height }, { { 16, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackLeftEighthBankToOrthogonalUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 96)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 100)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 104)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 108)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 97)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 101)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 105)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 109)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 98)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 102)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 106)),
                            { 0, 0, height }, { { 0, 0, height }, { 32, 16, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 110)),
                            { 0, 0, height }, { { 0, 16, height }, { 32, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::leftCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 99)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 103)),
                            { 0, 0, height }, { { 0, 31, height }, { 32, 1, 32 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 107)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 111)),
                            { 0, 0, height }, { { 0, 6, height }, { 32, 20, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 1 || direction == 2)
                {
                    PaintUtilPushTunnelRotated(session, direction, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                }
                PaintUtilSetSegmentSupportHeight(
                    session, PaintUtilRotateSegments(BlockedSegments::kStraightFlat, direction), 0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackRightEighthBankToOrthogonalUp25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        switch (trackSequence)
        {
            case 0:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 112)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::RightCorner, 8, height, session.SupportColours);
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 116)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::BottomCorner, 8, height, session.SupportColours);
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 120)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::LeftCorner, 8, height, session.SupportColours);
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 124)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        MetalASupportsPaintSetup(
                            session, supportType.metal, MetalSupportPlace::TopCorner, 8, height, session.SupportColours);
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 1:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 113)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 16, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 117)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 121)),
                            { 0, 0, height }, { { 0, 0, height + 32 }, { 32, 32, 1 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 125)),
                            { 0, 0, height }, { { 0, 16, height }, { 16, 16, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::topCorner, PaintSegment::rightCorner, PaintSegment::centre, PaintSegment::topLeftSide,
                            PaintSegment::topRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 2:
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::bottomCorner, PaintSegment::centre, PaintSegment::bottomLeftSide,
                            PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 3:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 114)),
                            { 0, 0, height }, { { 0, 0, height }, { 16, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 118)),
                            { 0, 0, height }, { { 31, 0, height }, { 1, 32, 32 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 122)),
                            { 0, 0, height }, { { 31, 0, height }, { 1, 32, 32 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 126)),
                            { 0, 0, height }, { { 16, 0, height }, { 16, 32, 3 } });
                        break;
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(
                            PaintSegment::leftCorner, PaintSegment::bottomCorner, PaintSegment::centre,
                            PaintSegment::topLeftSide, PaintSegment::bottomLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
            case 4:
                switch (direction)
                {
                    case 0:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 115)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                    case 1:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 119)),
                            { 0, 0, height }, { { 31, 0, height }, { 1, 32, 32 } });
                        break;
                    case 2:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 123)),
                            { 0, 0, height }, { { 31, 0, height }, { 1, 32, 32 } });
                        break;
                    case 3:
                        PaintAddImageAsParentRotated(
                            session, direction,
                            session.TrackColours.WithIndex((SPR_G2_VEKOMA_MINE_TRACK_GENTLE_LARGE_CURVE_BANKED + 127)),
                            { 0, 0, height }, { { 6, 0, height }, { 20, 32, 3 } });
                        break;
                }
                MetalASupportsPaintSetup(
                    session, supportType.metal, MetalSupportPlace::Centre, 8, height, session.SupportColours);
                if (direction == 0 || direction == 1)
                {
                    PaintUtilPushTunnelRotated(session, direction + 1, height + 8, kTunnelGroup, TunnelSubType::SlopeEnd);
                }
                PaintUtilSetSegmentSupportHeight(
                    session,
                    PaintUtilRotateSegments(
                        EnumsToFlags(PaintSegment::centre, PaintSegment::topLeftSide, PaintSegment::bottomRightSide),
                        direction),
                    0xFFFF, 0);
                PaintUtilSetGeneralSupportHeight(session, height + 72);
                break;
        }
    }

    static void VekomaMineRCTrackLeftEighthBankToDiagDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        uint8_t map[5] = { 4, 3, 1, 2, 0 };
        trackSequence = map[trackSequence];
        VekomaMineRCTrackRightEighthBankToOrthogonalUp25(
            session, ride, trackSequence, (direction + 1) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightEighthBankToDiagDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        uint8_t map[5] = { 4, 3, 1, 2, 0 };
        trackSequence = map[trackSequence];
        VekomaMineRCTrackLeftEighthBankToOrthogonalUp25(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackLeftEighthBankToOrthogonalDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        VekomaMineRCTrackRightEighthBankToDiagUp25(
            session, ride, trackSequence, (direction + 2) & 3, height, trackElement, supportType);
    }

    static void VekomaMineRCTrackRightEighthBankToOrthogonalDown25(
        PaintSession& session, const Ride& ride, uint8_t trackSequence, uint8_t direction, int32_t height,
        const TrackElement& trackElement, SupportType supportType)
    {
        trackSequence = mapLeftEighthTurnToOrthogonal[trackSequence];
        VekomaMineRCTrackLeftEighthBankToDiagUp25(
            session, ride, trackSequence, (direction + 3) & 3, height, trackElement, supportType);
    }

    TRACK_PAINT_FUNCTION GetTrackPaintFunction(int32_t trackType)
    {
        switch (trackType)
        {
            case TrackElemType::Flat:
                return VekomaMineRCTrackFlat;
            case TrackElemType::EndStation:
            case TrackElemType::BeginStation:
            case TrackElemType::MiddleStation:
                return VekomaMineRCTrackStation;
            case TrackElemType::Up25:
                return VekomaMineRCTrack25DegUp;
            case TrackElemType::Up60:
                return VekomaMineRCTrack60DegUp;
            case TrackElemType::FlatToUp25:
                return VekomaMineRCTrackFlatTo25DegUp;
            case TrackElemType::Up25ToUp60:
                return VekomaMineRCTrack25DegUpTo60DegUp;
            case TrackElemType::Up60ToUp25:
                return VekomaMineRCTrack60DegUpTo25DegUp;
            case TrackElemType::Up25ToFlat:
                return VekomaMineRCTrack25DegUpToFlat;
            case TrackElemType::Down25:
                return VekomaMineRCTrack25DegDown;
            case TrackElemType::Down60:
                return VekomaMineRCTrack60DegDown;
            case TrackElemType::FlatToDown25:
                return VekomaMineRCTrackFlatTo25DegDown;
            case TrackElemType::Down25ToDown60:
                return VekomaMineRCTrack25DegDownTo60DegDown;
            case TrackElemType::Down60ToDown25:
                return VekomaMineRCTrack60DegDownTo25DegDown;
            case TrackElemType::Down25ToFlat:
                return VekomaMineRCTrack25DegDownToFlat;
            
            case TrackElemType::LeftQuarterTurn5Tiles:
                return VekomaMineRCTrackLeftQuarterTurn5;
            case TrackElemType::RightQuarterTurn5Tiles:
                return VekomaMineRCTrackRightQuarterTurn5;
            case TrackElemType::FlatToLeftBank:
                return VekomaMineRCTrackFlatToLeftBank;
            case TrackElemType::FlatToRightBank:
                return VekomaMineRCTrackFlatToRightBank;
            case TrackElemType::LeftBankToFlat:
                return VekomaMineRCTrackLeftBankToFlat;
            case TrackElemType::RightBankToFlat:
                return VekomaMineRCTrackRightBankToFlat;
            case TrackElemType::BankedLeftQuarterTurn5Tiles:
                return VekomaMineRCTrackBankedLeftQuarterTurn5;
            case TrackElemType::BankedRightQuarterTurn5Tiles:
                return VekomaMineRCTrackBankedRightQuarterTurn5;
            case TrackElemType::LeftBankToUp25:
                return VekomaMineRCTrackLeftBankTo25DegUp;
            case TrackElemType::RightBankToUp25:
                return VekomaMineRCTrackRightBankTo25DegUp;
            case TrackElemType::Up25ToLeftBank:
                return VekomaMineRCTrack25DegUpToLeftBank;
            case TrackElemType::Up25ToRightBank:
                return VekomaMineRCTrack25DegUpToRightBank;
            case TrackElemType::LeftBankToDown25:
                return VekomaMineRCTrackLeftBankTo25DegDown;
            case TrackElemType::RightBankToDown25:
                return VekomaMineRCTrackRightBankTo25DegDown;
            case TrackElemType::Down25ToLeftBank:
                return VekomaMineRCTrack25DegDownToLeftBank;
            case TrackElemType::Down25ToRightBank:
                return VekomaMineRCTrack25DegDownToRightBank;
            case TrackElemType::LeftBank:
                return VekomaMineRCTrackLeftBank;
            case TrackElemType::RightBank:
                return VekomaMineRCTrackRightBank;
            case TrackElemType::LeftQuarterTurn5TilesUp25:
                return VekomaMineRCTrackLeftQuarterTurn525DegUp;
            case TrackElemType::RightQuarterTurn5TilesUp25:
                return VekomaMineRCTrackRightQuarterTurn525DegUp;
            case TrackElemType::LeftQuarterTurn5TilesDown25:
                return VekomaMineRCTrackLeftQuarterTurn525DegDown;
            case TrackElemType::RightQuarterTurn5TilesDown25:
                return VekomaMineRCTrackRightQuarterTurn525DegDown;
            case TrackElemType::SBendLeft:
                return VekomaMineRCTrackSBendLeft;
            case TrackElemType::SBendRight:
                return VekomaMineRCTrackSBendRight;
            case TrackElemType::LeftQuarterTurn3Tiles:
                return VekomaMineRCTrackLeftQuarterTurn3;
            case TrackElemType::RightQuarterTurn3Tiles:
                return VekomaMineRCTrackRightQuarterTurn3;
            case TrackElemType::LeftBankedQuarterTurn3Tiles:
                return VekomaMineRCTrackLeftQuarterTurn3Bank;
            case TrackElemType::RightBankedQuarterTurn3Tiles:
                return VekomaMineRCTrackRightQuarterTurn3Bank;
            case TrackElemType::LeftQuarterTurn3TilesUp25:
                return VekomaMineRCTrackLeftQuarterTurn3Tile25DegUp;
            case TrackElemType::RightQuarterTurn3TilesUp25:
                return VekomaMineRCTrackRightQuarterTurn3Tile25DegUp;
            case TrackElemType::LeftQuarterTurn3TilesDown25:
                return VekomaMineRCTrackLeftQuarterTurn3Tile25DegDown;
            case TrackElemType::RightQuarterTurn3TilesDown25:
                return VekomaMineRCTrackRightQuarterTurn3Tile25DegDown;
            case TrackElemType::LeftHalfBankedHelixUpSmall:
                return VekomaMineRCTrackLeftHalfBankedHelixUpSmall;
            case TrackElemType::RightHalfBankedHelixUpSmall:
                return VekomaMineRCTrackRightHalfBankedHelixUpSmall;
            case TrackElemType::LeftHalfBankedHelixDownSmall:
                return VekomaMineRCTrackLeftHalfBankedHelixDownSmall;
            case TrackElemType::RightHalfBankedHelixDownSmall:
                return VekomaMineRCTrackRightHalfBankedHelixDownSmall;
            case TrackElemType::LeftHalfBankedHelixUpLarge:
                return VekomaMineRCTrackLeftHalfBankedHelixUpLarge;
            case TrackElemType::RightHalfBankedHelixUpLarge:
                return VekomaMineRCTrackRightHalfBankedHelixUpLarge;
            case TrackElemType::LeftHalfBankedHelixDownLarge:
                return VekomaMineRCTrackLeftHalfBankedHelixDownLarge;
            case TrackElemType::RightHalfBankedHelixDownLarge:
                return VekomaMineRCTrackRightHalfBankedHelixDownLarge;
            case TrackElemType::LeftQuarterTurn1TileUp60:
                return VekomaMineRCTrackLeftQuarterTurn160DegUp;
            case TrackElemType::RightQuarterTurn1TileUp60:
                return VekomaMineRCTrackRightQuarterTurn160DegUp;
            case TrackElemType::LeftQuarterTurn1TileDown60:
                return VekomaMineRCTrackLeftQuarterTurn160DegDown;
            case TrackElemType::RightQuarterTurn1TileDown60:
                return VekomaMineRCTrackRightQuarterTurn160DegDown;
            case TrackElemType::Brakes:
                return VekomaMineRCTrackBrakes;
            case TrackElemType::Up25LeftBanked:
                return VekomaMineRCTrack25DegUpLeftBanked;
            case TrackElemType::Up25RightBanked:
                return VekomaMineRCTrack25DegUpRightBanked;
            case TrackElemType::OnRidePhoto:
                return VekomaMineRCTrackOnRidePhoto;
            case TrackElemType::Down25LeftBanked:
                return VekomaMineRCTrack25DegDownLeftBanked;
            case TrackElemType::Down25RightBanked:
                return VekomaMineRCTrack25DegDownRightBanked;
            case TrackElemType::FlatToUp60LongBase:
                return VekomaMineRCTrackFlatTo60DegUpLongBase;
            case TrackElemType::Up60ToFlatLongBase:
                return VekomaMineRCTrack60DegUpToFlatLongBase;
            case TrackElemType::FlatToDown60LongBase:
                return VekomaMineRCTrackFlatTo60DegDownLongBase;
            case TrackElemType::Down60ToFlatLongBase:
                return VekomaMineRCTrack60DegDownToFlatLongBase;
            case TrackElemType::LeftEighthToDiag:
                return VekomaMineRCTrackLeftEighthToDiag;
            case TrackElemType::RightEighthToDiag:
                return VekomaMineRCTrackRightEighthToDiag;
            case TrackElemType::LeftEighthToOrthogonal:
                return VekomaMineRCTrackLeftEighthToOrthogonal;
            case TrackElemType::RightEighthToOrthogonal:
                return VekomaMineRCTrackRightEighthToOrthogonal;
            case TrackElemType::LeftEighthBankToDiag:
                return VekomaMineRCTrackLeftEighthBankToDiag;
            case TrackElemType::RightEighthBankToDiag:
                return VekomaMineRCTrackRightEighthBankToDiag;
            case TrackElemType::LeftEighthBankToOrthogonal:
                return VekomaMineRCTrackLeftEighthBankToOrthogonal;
            case TrackElemType::RightEighthBankToOrthogonal:
                return VekomaMineRCTrackRightEighthBankToOrthogonal;
            case TrackElemType::DiagFlat:
                return VekomaMineRCTrackDiagFlat;
            case TrackElemType::DiagBrakes:
                return VekomaMineRCTrackDiagBrakes;
            case TrackElemType::DiagBlockBrakes:
                return VekomaMineRCTrackDiagBlockBrakes;
            case TrackElemType::DiagUp25:
                return VekomaMineRCTrackDiag25DegUp;
            case TrackElemType::DiagUp60:
                return VekomaMineRCTrackDiag60DegUp;
            case TrackElemType::DiagFlatToUp25:
                return VekomaMineRCTrackDiagFlatTo25DegUp;
            case TrackElemType::DiagUp25ToUp60:
                return VekomaMineRCTrackDiag25DegUpTo60DegUp;
            case TrackElemType::DiagUp60ToUp25:
                return VekomaMineRCTrackDiag60DegUpTo25DegUp;
            case TrackElemType::DiagUp25ToFlat:
                return VekomaMineRCTrackDiag25DegUpToFlat;
            case TrackElemType::DiagDown25:
                return VekomaMineRCTrackDiag25DegDown;
            case TrackElemType::DiagDown60:
                return VekomaMineRCTrackDiag60DegDown;
            case TrackElemType::DiagFlatToDown25:
                return VekomaMineRCTrackDiagFlatTo25DegDown;
            case TrackElemType::DiagDown25ToDown60:
                return VekomaMineRCTrackDiag25DegDownTo60DegDown;
            case TrackElemType::DiagDown60ToDown25:
                return VekomaMineRCTrackDiag60DegDownTo25DegDown;
            case TrackElemType::DiagDown25ToFlat:
                return VekomaMineRCTrackDiag25DegDownToFlat;
            case TrackElemType::DiagFlatToLeftBank:
                return VekomaMineRCTrackDiagFlatToLeftBank;
            case TrackElemType::DiagFlatToRightBank:
                return VekomaMineRCTrackDiagFlatToRightBank;
            case TrackElemType::DiagLeftBankToFlat:
                return VekomaMineRCTrackDiagLeftBankToFlat;
            case TrackElemType::DiagRightBankToFlat:
                return VekomaMineRCTrackDiagRightBankToFlat;
            case TrackElemType::DiagLeftBankToUp25:
                return VekomaMineRCTrackDiagLeftBankTo25DegUp;
            case TrackElemType::DiagRightBankToUp25:
                return VekomaMineRCTrackDiagRightBankTo25DegUp;
            case TrackElemType::DiagUp25ToLeftBank:
                return VekomaMineRCTrackDiag25DegUpToLeftBank;
            case TrackElemType::DiagUp25ToRightBank:
                return VekomaMineRCTrackDiag25DegUpToRightBank;
            case TrackElemType::DiagLeftBankToDown25:
                return VekomaMineRCTrackDiagLeftBankTo25DegDown;
            case TrackElemType::DiagRightBankToDown25:
                return VekomaMineRCTrackDiagRightBankTo25DegDown;
            case TrackElemType::DiagDown25ToLeftBank:
                return VekomaMineRCTrackDiag25DegDownToLeftBank;
            case TrackElemType::DiagDown25ToRightBank:
                return VekomaMineRCTrackDiag25DegDownToRightBank;
            case TrackElemType::DiagLeftBank:
                return VekomaMineRCTrackDiagLeftBank;
            case TrackElemType::DiagRightBank:
                return VekomaMineRCTrackDiagRightBank;
            case TrackElemType::BlockBrakes:
                return VekomaMineRCTrackBlockBrakes;
            case TrackElemType::LeftBankedQuarterTurn3TileUp25:
                return VekomaMineRCTrackLeftBankedQuarterTurn3Tile25DegUp;
            case TrackElemType::RightBankedQuarterTurn3TileUp25:
                return VekomaMineRCTrackRightBankedQuarterTurn3Tile25DegUp;
            case TrackElemType::LeftBankedQuarterTurn3TileDown25:
                return VekomaMineRCTrackLeftBankedQuarterTurn3Tile25DegDown;
            case TrackElemType::RightBankedQuarterTurn3TileDown25:
                return VekomaMineRCTrackRightBankedQuarterTurn3Tile25DegDown;
            case TrackElemType::LeftBankedQuarterTurn5TileUp25:
                return VekomaMineRCTrackLeftBankedQuarterTurn525DegUp;
            case TrackElemType::RightBankedQuarterTurn5TileUp25:
                return VekomaMineRCTrackRightBankedQuarterTurn525DegUp;
            case TrackElemType::LeftBankedQuarterTurn5TileDown25:
                return VekomaMineRCTrackLeftBankedQuarterTurn525DegDown;
            case TrackElemType::RightBankedQuarterTurn5TileDown25:
                return VekomaMineRCTrackRightBankedQuarterTurn525DegDown;
            case TrackElemType::Up25ToLeftBankedUp25:
                return VekomaMineRCTrack25DegUpToLeftBanked25DegUp;
            case TrackElemType::Up25ToRightBankedUp25:
                return VekomaMineRCTrack25DegUpToRightBanked25DegUp;
            case TrackElemType::LeftBankedUp25ToUp25:
                return VekomaMineRCTrackLeftBanked25DegUpTo25DegUp;
            case TrackElemType::RightBankedUp25ToUp25:
                return VekomaMineRCTrackRightBanked25DegUpTo25DegUp;
            case TrackElemType::Down25ToLeftBankedDown25:
                return VekomaMineRCTrack25DegDownToLeftBanked25DegDown;
            case TrackElemType::Down25ToRightBankedDown25:
                return VekomaMineRCTrack25DegDownToRightBanked25DegDown;
            case TrackElemType::LeftBankedDown25ToDown25:
                return VekomaMineRCTrackLeftBanked25DegDownTo25DegDown;
            case TrackElemType::RightBankedDown25ToDown25:
                return VekomaMineRCTrackRightBanked25DegDownTo25DegDown;
            case TrackElemType::LeftBankedFlatToLeftBankedUp25:
                return VekomaMineRCTrackLeftBankedFlatToLeftBanked25DegUp;
            case TrackElemType::RightBankedFlatToRightBankedUp25:
                return VekomaMineRCTrackRightBankedFlatToRightBanked25DegUp;
            case TrackElemType::LeftBankedUp25ToLeftBankedFlat:
                return VekomaMineRCTrackLeftBanked25DegUpToLeftBankedFlat;
            case TrackElemType::RightBankedUp25ToRightBankedFlat:
                return VekomaMineRCTrackRightBanked25DegUpToRightBankedFlat;
            case TrackElemType::LeftBankedFlatToLeftBankedDown25:
                return VekomaMineRCTrackLeftBankedFlatToLeftBanked25DegDown;
            case TrackElemType::RightBankedFlatToRightBankedDown25:
                return VekomaMineRCTrackRightBankedFlatToRightBanked25DegDown;
            case TrackElemType::LeftBankedDown25ToLeftBankedFlat:
                return VekomaMineRCTrackLeftBanked25DegDownToLeftBankedFlat;
            case TrackElemType::RightBankedDown25ToRightBankedFlat:
                return VekomaMineRCTrackRightBanked25DegDownToRightBankedFlat;
            case TrackElemType::FlatToLeftBankedUp25:
                return VekomaMineRCTrackFlatToLeftBanked25DegUp;
            case TrackElemType::FlatToRightBankedUp25:
                return VekomaMineRCTrackFlatToRightBanked25DegUp;
            case TrackElemType::LeftBankedUp25ToFlat:
                return VekomaMineRCTrackLeftBanked25DegUpToFlat;
            case TrackElemType::RightBankedUp25ToFlat:
                return VekomaMineRCTrackRightBanked25DegUpToFlat;
            case TrackElemType::FlatToLeftBankedDown25:
                return VekomaMineRCTrackFlatToLeftBanked25DegDown;
            case TrackElemType::FlatToRightBankedDown25:
                return VekomaMineRCTrackFlatToRightBanked25DegDown;
            case TrackElemType::LeftBankedDown25ToFlat:
                return VekomaMineRCTrackLeftBanked25DegDownToFlat;
            case TrackElemType::RightBankedDown25ToFlat:
                return VekomaMineRCTrackRightBanked25DegDownToFlat;
                        
            case TrackElemType::LeftBankToLeftQuarterTurn3TilesUp25:
                return VekomaMineRCTrackLeftBankToLeftQuarterTurn3Tile25DegUp;
            case TrackElemType::RightBankToRightQuarterTurn3TilesUp25:
                return VekomaMineRCTrackRightBankToRightQuarterTurn3Tile25DegUp;
            case TrackElemType::LeftQuarterTurn3TilesDown25ToLeftBank:
                return VekomaMineRCTrackLeftQuarterTurn3Tile25DegDownToLeftBank;
            case TrackElemType::RightQuarterTurn3TilesDown25ToRightBank:
                return VekomaMineRCTrackRightQuarterTurn3Tile25DegDownToRightBank;
            
            case TrackElemType::FlatToUp60:
                return VekomaMineRCTrackFlatTo60DegUp;
            case TrackElemType::Up60ToFlat:
                return VekomaMineRCTrack60DegUpToFlat;
            case TrackElemType::FlatToDown60:
                return VekomaMineRCTrackFlatTo60DegDown;
            case TrackElemType::Down60ToFlat:
                return VekomaMineRCTrack60DegDownToFlat;
            case TrackElemType::DiagFlatToUp60:
                return VekomaMineRCTrackDiagFlatTo60DegUp;
            case TrackElemType::DiagUp60ToFlat:
                return VekomaMineRCTrackDiag60DegUpToFlat;
            case TrackElemType::DiagFlatToDown60:
                return VekomaMineRCTrackDiagFlatTo60DegDown;
            case TrackElemType::DiagDown60ToFlat:
                return VekomaMineRCTrackDiag60DegDownToFlat;

            case TrackElemType::LeftEighthToDiagUp25:
                return VekomaMineRCTrackLeftEighthToDiagUp25;
            case TrackElemType::RightEighthToDiagUp25:
                return VekomaMineRCTrackRightEighthToDiagUp25;
            case TrackElemType::LeftEighthToDiagDown25:
                return VekomaMineRCTrackLeftEighthToDiagDown25;
            case TrackElemType::RightEighthToDiagDown25:
                return VekomaMineRCTrackRightEighthToDiagDown25;
            case TrackElemType::LeftEighthToOrthogonalUp25:
                return VekomaMineRCTrackLeftEighthToOrthogonalUp25;
            case TrackElemType::RightEighthToOrthogonalUp25:
                return VekomaMineRCTrackRightEighthToOrthogonalUp25;
            case TrackElemType::LeftEighthToOrthogonalDown25:
                return VekomaMineRCTrackLeftEighthToOrthogonalDown25;
            case TrackElemType::RightEighthToOrthogonalDown25:
                return VekomaMineRCTrackRightEighthToOrthogonalDown25;

            case TrackElemType::DiagUp25ToLeftBankedUp25:
                return VekomaMineRCTrackDiagUp25ToLeftBankedUp25;
            case TrackElemType::DiagUp25ToRightBankedUp25:
                return VekomaMineRCTrackDiagUp25ToRightBankedUp25;
            case TrackElemType::DiagLeftBankedUp25ToUp25:
                return VekomaMineRCTrackDiagLeftBankedUp25ToUp25;
            case TrackElemType::DiagRightBankedUp25ToUp25:
                return VekomaMineRCTrackDiagRightBankedUp25ToUp25;
            case TrackElemType::DiagDown25ToLeftBankedDown25:
                return VekomaMineRCTrackDiagDown25ToLeftBankedDown25;
            case TrackElemType::DiagDown25ToRightBankedDown25:
                return VekomaMineRCTrackDiagDown25ToRightBankedDown25;
            case TrackElemType::DiagLeftBankedDown25ToDown25:
                return VekomaMineRCTrackDiagLeftBankedDown25ToDown25;
            case TrackElemType::DiagRightBankedDown25ToDown25:
                return VekomaMineRCTrackDiagRightBankedDown25ToDown25;

            case TrackElemType::DiagLeftBankedFlatToLeftBankedUp25:
                return VekomaMineRCTrackDiagLeftBankedFlatToLeftBankedUp25;
            case TrackElemType::DiagRightBankedFlatToRightBankedUp25:
                return VekomaMineRCTrackDiagRightBankedFlatToRightBankedUp25;
            case TrackElemType::DiagLeftBankedUp25ToLeftBankedFlat:
                return VekomaMineRCTrackDiagLeftBankedUp25ToLeftBankedFlat;
            case TrackElemType::DiagRightBankedUp25ToRightBankedFlat:
                return VekomaMineRCTrackDiagRightBankedUp25ToRightBankedFlat;
            case TrackElemType::DiagLeftBankedFlatToLeftBankedDown25:
                return VekomaMineRCTrackDiagLeftBankedFlatToLeftBankedDown25;
            case TrackElemType::DiagRightBankedFlatToRightBankedDown25:
                return VekomaMineRCTrackDiagRightBankedFlatToRightBankedDown25;
            case TrackElemType::DiagLeftBankedDown25ToLeftBankedFlat:
                return VekomaMineRCTrackDiagLeftBankedDown25ToLeftBankedFlat;
            case TrackElemType::DiagRightBankedDown25ToRightBankedFlat:
                return VekomaMineRCTrackDiagRightBankedDown25ToRightBankedFlat;
            case TrackElemType::DiagUp25LeftBanked:
                return VekomaMineRCTrackDiagUp25LeftBanked;
            case TrackElemType::DiagUp25RightBanked:
                return VekomaMineRCTrackDiagUp25RightBanked;
            case TrackElemType::DiagDown25LeftBanked:
                return VekomaMineRCTrackDiagDown25LeftBanked;
            case TrackElemType::DiagDown25RightBanked:
                return VekomaMineRCTrackDiagDown25RightBanked;
            case TrackElemType::DiagFlatToLeftBankedUp25:
                return VekomaMineRCTrackDiagFlatToLeftBankedUp25;
            case TrackElemType::DiagFlatToRightBankedUp25:
                return VekomaMineRCTrackDiagFlatToRightBankedUp25;
            case TrackElemType::DiagLeftBankedUp25ToFlat:
                return VekomaMineRCTrackDiagLeftBankedUp25ToFlat;
            case TrackElemType::DiagRightBankedUp25ToFlat:
                return VekomaMineRCTrackDiagRightBankedUp25ToFlat;
            case TrackElemType::DiagFlatToLeftBankedDown25:
                return VekomaMineRCTrackDiagFlatToLeftBankedDown25;
            case TrackElemType::DiagFlatToRightBankedDown25:
                return VekomaMineRCTrackDiagFlatToRightBankedDown25;
            case TrackElemType::DiagLeftBankedDown25ToFlat:
                return VekomaMineRCTrackDiagLeftBankedDown25ToFlat;
            case TrackElemType::DiagRightBankedDown25ToFlat:
                return VekomaMineRCTrackDiagRightBankedDown25ToFlat;
            case TrackElemType::LeftEighthBankToDiagUp25:
                return VekomaMineRCTrackLeftEighthBankToDiagUp25;
            case TrackElemType::RightEighthBankToDiagUp25:
                return VekomaMineRCTrackRightEighthBankToDiagUp25;
            case TrackElemType::LeftEighthBankToDiagDown25:
                return VekomaMineRCTrackLeftEighthBankToDiagDown25;
            case TrackElemType::RightEighthBankToDiagDown25:
                return VekomaMineRCTrackRightEighthBankToDiagDown25;
            case TrackElemType::LeftEighthBankToOrthogonalUp25:
                return VekomaMineRCTrackLeftEighthBankToOrthogonalUp25;
            case TrackElemType::RightEighthBankToOrthogonalUp25:
                return VekomaMineRCTrackRightEighthBankToOrthogonalUp25;
            case TrackElemType::LeftEighthBankToOrthogonalDown25:
                return VekomaMineRCTrackLeftEighthBankToOrthogonalDown25;
            case TrackElemType::RightEighthBankToOrthogonalDown25:
                return VekomaMineRCTrackRightEighthBankToOrthogonalDown25;
        }
        return nullptr;
    }

} // namespace OpenRCT2::SingleRailRC
