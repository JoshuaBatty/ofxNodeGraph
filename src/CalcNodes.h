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
#include "NodeTypes.h"

//------------------------
class OutputNode : public CalcNode
{
public:
    OutputNode(ECalcNodeType _t) : CalcNode(_t){};
    void update() override;
    float Passthrough(float a){ return a; }
    static OutputNode* CAST(gtf::Node* node){ return dynamic_cast<OutputNode*>(node); }
};

class CalcOutputNode : public OutputNode
{
public:
    CalcOutputNode() : OutputNode(ECalcNodeType::OUTPUT){};
    float Passthrough(float a){ return a; }
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
