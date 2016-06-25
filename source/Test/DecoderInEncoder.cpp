//
//  DecoderInEncoder.cpp
//  HM
//
//  Created by MacBook on 21.06.16.
//
//

#include "DecoderInEncoder.hpp"


//Void
//TDecCu::xReconIntraQT( TComDataCU* pcCU, UInt uiDepth )
//{
//    if (pcCU->getIPCMFlag(0))
//    {
//        xReconPCM( pcCU, uiDepth );
//        return;
//    }
//    const UInt numChType = pcCU->getPic()->getChromaFormat()!=CHROMA_400 ? 2 : 1;
//    for (UInt chType=CHANNEL_TYPE_LUMA; chType<numChType; chType++)
//    {
//        const ChannelType chanType=ChannelType(chType);
//        const Bool NxNPUHas4Parts = ::isChroma(chanType) ? enable4ChromaPUsInIntraNxNCU(pcCU->getPic()->getChromaFormat()) : true;
//        const UInt uiInitTrDepth = ( pcCU->getPartitionSize(0) != SIZE_2Nx2N && NxNPUHas4Parts ? 1 : 0 );
//        
//        TComTURecurse tuRecurseCU(pcCU, 0);
//        TComTURecurse tuRecurseWithPU(tuRecurseCU, false, (uiInitTrDepth==0)?TComTU::DONT_SPLIT : TComTU::QUAD_SPLIT);
//        
//        do
//        {
//            xIntraRecQT( m_ppcYuvReco[uiDepth], m_ppcYuvReco[uiDepth], m_ppcYuvResi[uiDepth], chanType, tuRecurseWithPU );
//        } while (tuRecurseWithPU.nextSection(tuRecurseCU));
//    }
//}

//Void
//TDecCu::xIntraRecQT(TComYuv*    pcRecoYuv,
//                    TComYuv*    pcPredYuv,
//                    TComYuv*    pcResiYuv,
//                    const ChannelType chType,
//                    TComTU     &rTu)
//{
//    UInt uiTrDepth    = rTu.GetTransformDepthRel();
//    TComDataCU *pcCU  = rTu.getCU();
//    UInt uiAbsPartIdx = rTu.GetAbsPartIdxTU();
//    UInt uiTrMode     = pcCU->getTransformIdx( uiAbsPartIdx );
//    if( uiTrMode == uiTrDepth )
//    {
//        if (isLuma(chType))
//        {
//            xIntraRecBlk( pcRecoYuv, pcPredYuv, pcResiYuv, COMPONENT_Y,  rTu );
//        }
//        else
//        {
//            const UInt numValidComp=getNumberValidComponents(rTu.GetChromaFormat());
//            for(UInt compID=COMPONENT_Cb; compID<numValidComp; compID++)
//            {
//                xIntraRecBlk( pcRecoYuv, pcPredYuv, pcResiYuv, ComponentID(compID), rTu );
//            }
//        }
//    }
//    else
//    {
//        TComTURecurse tuRecurseChild(rTu, false);
//        do
//        {
//            xIntraRecQT( pcRecoYuv, pcPredYuv, pcResiYuv, chType, tuRecurseChild );
//        } while (tuRecurseChild.nextSection(rTu));
//    }
//}








Void DecoderInEncoder::decompressCtu( TComDataCU* pCtu )
{
    xDecompressCU( pCtu, 0,  0 );
}

Void DecoderInEncoder::xDecompressCU( TComDataCU* pCtu, UInt uiAbsPartIdx,  UInt uiDepth )
{
    TComPic* pcPic = pCtu->getPic();
    TComSlice * pcSlice = pCtu->getSlice();
    const TComSPS &sps=*(pcSlice->getSPS());
    
    Bool bBoundary = false;
    UInt uiLPelX   = pCtu->getCUPelX() + g_auiRasterToPelX[ g_auiZscanToRaster[uiAbsPartIdx] ];
    UInt uiRPelX   = uiLPelX + (sps.getMaxCUWidth()>>uiDepth)  - 1;
    UInt uiTPelY   = pCtu->getCUPelY() + g_auiRasterToPelY[ g_auiZscanToRaster[uiAbsPartIdx] ];
    UInt uiBPelY   = uiTPelY + (sps.getMaxCUHeight()>>uiDepth) - 1;
    
    if( ( uiRPelX >= sps.getPicWidthInLumaSamples() ) || ( uiBPelY >= sps.getPicHeightInLumaSamples() ) )
    {
        bBoundary = true;
    }
    
    if( ( ( uiDepth < pCtu->getDepth( uiAbsPartIdx ) ) && ( uiDepth < sps.getLog2DiffMaxMinCodingBlockSize() ) ) || bBoundary )
    {
        UInt uiNextDepth = uiDepth + 1;
        UInt uiQNumParts = pCtu->getTotalNumPart() >> (uiNextDepth<<1);
        UInt uiIdx = uiAbsPartIdx;
        for ( UInt uiPartIdx = 0; uiPartIdx < 4; uiPartIdx++ )
        {
            uiLPelX = pCtu->getCUPelX() + g_auiRasterToPelX[ g_auiZscanToRaster[uiIdx] ];
            uiTPelY = pCtu->getCUPelY() + g_auiRasterToPelY[ g_auiZscanToRaster[uiIdx] ];
            
            if( ( uiLPelX < sps.getPicWidthInLumaSamples() ) && ( uiTPelY < sps.getPicHeightInLumaSamples() ) )
            {
                xDecompressCU(pCtu, uiIdx, uiNextDepth );
            }
            
            uiIdx += uiQNumParts;
        }
        return;
    }
//    
//    // Residual reconstruction
//    m_ppcYuvResi[uiDepth]->clear();
//    
//    TEncCu::m_ppcY
//    
//    m_ppcCU[uiDepth]->copySubCU( pCtu, uiAbsPartIdx );
//    
//    switch( m_ppcCU[uiDepth]->getPredictionMode(0) )
//    {
//        case MODE_INTER:
//            xReconInter( m_ppcCU[uiDepth], uiDepth );
//            break;
//        case MODE_INTRA:
//            xReconIntraQT( m_ppcCU[uiDepth], uiDepth );
//            break;
//        default:
//            assert(0);
//            break;
//    }
//    
//#if DEBUG_STRING
//    const PredMode predMode=m_ppcCU[uiDepth]->getPredictionMode(0);
//    if (DebugOptionList::DebugString_Structure.getInt()&DebugStringGetPredModeMask(predMode))
//    {
//        PartSize eSize=m_ppcCU[uiDepth]->getPartitionSize(0);
//        std::ostream &ss(std::cout);
//        
//        ss <<"###: " << (predMode==MODE_INTRA?"Intra   ":"Inter   ") << partSizeToString[eSize] << " CU at " << m_ppcCU[uiDepth]->getCUPelX() << ", " << m_ppcCU[uiDepth]->getCUPelY() << " width=" << UInt(m_ppcCU[uiDepth]->getWidth(0)) << std::endl;
//    }
//#endif
//    
//    if ( m_ppcCU[uiDepth]->isLosslessCoded(0) && (m_ppcCU[uiDepth]->getIPCMFlag(0) == false))
//    {
//        xFillPCMBuffer(m_ppcCU[uiDepth], uiDepth);
//    }
//    
//    xCopyToPic( m_ppcCU[uiDepth], pcPic, uiAbsPartIdx, uiDepth );
}