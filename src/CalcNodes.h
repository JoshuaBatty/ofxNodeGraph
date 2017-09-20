//
//  CalcNodes.h
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#pragma once

#include "ofMain.h"
#include <NodeGraph.h>
#include "Waveforms.h"

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
    WAVEFORM
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

//------------------------
class CalcWaveNode : public CalcNode
{
public:
    CalcWaveNode(ECalcNodeType _t) : CalcNode(_t){};
    bool dirty { true };

    void update() override;
    virtual float Result(float x) { return 0; };
    Waveforms waves;
    float amp_min { -1.0 };
    float amp_max { 1.0 };
    float speed { 1.0 };
    
    static CalcWaveNode* CAST(gtf::Node* node){ return dynamic_cast<CalcWaveNode*>(node); }
};

class CalcSineNode : public CalcWaveNode {
public:
    CalcSineNode() : CalcWaveNode(ECalcNodeType::WAVEFORM){};
    float Result(float x) { return waves.sine.result(x); }
};
class CalcTriangleNode : public CalcWaveNode {
public:
    CalcTriangleNode() : CalcWaveNode(ECalcNodeType::WAVEFORM){};
    float Result(float x) { return waves.tri.result(x); }
};
class CalcSquareNode : public CalcWaveNode {
public:
    CalcSquareNode() : CalcWaveNode(ECalcNodeType::WAVEFORM){};
    float Result(float x) { return waves.pulse.result(x); }
};
class CalcSawNode : public CalcWaveNode {
public:
    CalcSawNode() : CalcWaveNode(ECalcNodeType::WAVEFORM){};
    float Result(float x) { return waves.saw.result(x); }
};
class CalcNoiseNode : public CalcWaveNode {
public:
    CalcNoiseNode() : CalcWaveNode(ECalcNodeType::WAVEFORM){};
    float Result(float x) { return waves.noise.result(x); }
};

//-------------------------

class CalcNumberNode : public CalcNode
{
public:
    CalcNumberNode(ECalcNodeType _t) : CalcNode(_t){};
    CalcNumberNode() : CalcNode(ECalcNodeType::CNT_NUMBER)
    {
        number = rand() % 60;
    };
    
    void update() override;
};

class CalcMathOpNode : public CalcNumberNode
{
public:
    CalcMathOpNode(ECalcNodeType _t) : CalcNumberNode(_t){};
    void update() override;
    
    virtual float MathOp(float a, float b){ return 0; }
};

class CalcAddNode : public CalcMathOpNode
{
public:
    CalcAddNode() : CalcMathOpNode(ECalcNodeType::CNT_ADD){};
    float MathOp(float a, float b) override { return a + b; }
};

class CalcSubstractNode : public CalcMathOpNode
{
public:
    CalcSubstractNode() : CalcMathOpNode(ECalcNodeType::CNT_SUBSTRACT){};
    float MathOp(float a, float b){ return a - b; }
};

class CalcMultiplyNode : public CalcMathOpNode
{
public:
    CalcMultiplyNode() : CalcMathOpNode(ECalcNodeType::CNT_MULTIPLY){};
    float MathOp(float a, float b){ return a * b; }
};

class CalcDivideNode : public CalcMathOpNode
{
public:
    CalcDivideNode() : CalcMathOpNode(ECalcNodeType::CNT_DIVIDE){};
    float MathOp(float a, float b){ return (b == 0) ? 0 : a / b; }
};
