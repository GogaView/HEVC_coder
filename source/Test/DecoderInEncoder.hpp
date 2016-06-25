//
//  DecoderInEncoder.hpp
//  HM
//
//  Created by MacBook on 21.06.16.
//
//

#ifndef DecoderInEncoder_hpp
#define DecoderInEncoder_hpp

#include <stdio.h>
#include "TEncCu.h"

class DecoderInEncoder
{
    friend class TEncCu;
    
    
public:
    Void            decompressCtu( TComDataCU* pCtu );
    Void            xDecompressCU( TComDataCU* pCtu, UInt uiAbsPartIdx,  UInt uiDepth );
    
};

#endif /* DecoderInEncoder_hpp */
