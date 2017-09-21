//
//  NodeTypes.h
//  example
//
//  Created by Joshua Batty on 21/9/17.
//
//

#ifndef NodeTypes_h
#define NodeTypes_h

#define MAX_CONNECTION_COUNT 32

enum class ECalcConnectionType : uint8_t
{
    CCT_INT32
};

enum class ECalcNodeType : uint8_t
{
    CNT_UNDEFINED,
    CNT_NUMBER,
    CNT_ADD,
    CNT_SUBSTRACT,
    CNT_MULTIPLY,
    CNT_DIVIDE,
    WAVEFORM,
    OUTPUT
};

struct ConnectionDesc
{
    const char* name;
    ECalcConnectionType type;
};

class CalcNode : public gtf::Node
{
public:
    CalcNode(ECalcNodeType _t) : type(_t){};
    
    bool dirty { true };
    ECalcNodeType type;
    float number { 0.0 };
    
    static CalcNode* CAST(gtf::Node* node){ return dynamic_cast<CalcNode*>(node); }
};

#endif /* NodeTypes_h */
