//
//  FileReader.h
//  HM
//
//  Created by MacBook on 07.02.17.
//
//

#ifndef FileReader_h
#define FileReader_h


#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>


struct st_config
{
    int iIndex;
    
    int iFrame;
    int iTrSize;
    int TrW;
    int TrH;
    
    int TrX;
    int TrY;
    int Offset;
    
    
    bool operator==(const st_config& val) const
    {
        return  iFrame == val.iFrame &&
                iTrSize == val.iTrSize &&
                TrW == val.TrW &&
                TrH == val.TrH &&
                TrX == val.TrH &&
                TrY == val.TrY;
    }
    
    bool operator!=(const st_config& val) const
    {
        return  !(iFrame == val.iFrame &&
        iTrSize == val.iTrSize &&
        TrW == val.TrW &&
        TrH == val.TrH &&
        TrX == val.TrH &&
        TrY == val.TrY);
    }
};

class cfg
{
public:
    static std::vector<st_config>   vCONFIG;
    static int                      s_CurrFrame;
};




inline void openFile(std::string strFile)
{
    std::ifstream file;
    file.open(strFile.c_str());
    
    std::string strLine;
    std::getline(file,strLine);
    
    while (std::getline(file,strLine))
    {
        st_config data;
        
        std::stringstream ss(strLine);
        data.iIndex  = cfg::vCONFIG.size();
        
        ss >> data.iFrame;
        ss >> data.iTrSize;
        ss >> data.TrW;
        ss >> data.TrH;
        ss >> data.TrX;
        ss >> data.TrY;
        ss >> data.Offset;
        
        cfg::vCONFIG.push_back(data);
    }
}

inline bool isCorrectPosition(int i1, int i2, int i3, int i4, int i5, int i6)
{
    st_config data;
    
    data.iFrame = cfg::s_CurrFrame;
    data.iTrSize = i1;
    data.TrW = i2;
    data.TrH = i3;
    data.TrX = i4;
    data.TrY = i5;
    data.Offset = i6;
    
    auto copy =  cfg::vCONFIG;
    
    for(int i = 0; i != copy.size(); i++)
    {
        auto data1 = cfg::vCONFIG[i];
        if( (data1).iFrame == data.iFrame &&
            (data1).iTrSize == data.iTrSize &&
            (data1).TrW == data.TrW &&
            (data1).TrH == data.TrH &&
            (data1).TrX == data.TrX &&
            (data1).TrY == data.TrY &&
            (data1).Offset == data.Offset )
        {
            if(data1.iIndex == 35)
                int i = 0;
            
//            cfg::vCONFIG.erase(cfg::vCONFIG.begin()+i);
            
            return true;
        }
    }
    return false;
}


#endif /* FileReader_h */
