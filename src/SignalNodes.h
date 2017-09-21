//
//  SignalNodes.h
//  example
//
//  Created by Joshua Batty on 21/9/17.
//
//

#pragma once

#include "ofMain.h"
#include <NodeGraph.h>
#include "Waveforms.h"
#include "NodeTypes.h"


//------------------------
class SignalNode : public CalcNode
{
public:
    SignalNode(ECalcNodeType _t) : CalcNode(_t){};
    
    void update() override;
    virtual float Result(float x) { return 0; };
    Waveforms waves;
    float amp_min { -1.0 };
    float amp_max { 1.0 };
    float speed { 1.0 };
    
    static SignalNode* CAST(gtf::Node* node){ return dynamic_cast<SignalNode*>(node); }
};

class SignalSineNode : public SignalNode {
public:
    SignalSineNode() : SignalNode(ECalcNodeType::WAVEFORM){};
    float Result(float x) { return waves.sine.result(x); }
};
class SignalTriangleNode : public SignalNode {
public:
    SignalTriangleNode() : SignalNode(ECalcNodeType::WAVEFORM){};
    float Result(float x) { return waves.tri.result(x); }
};
class SignalSquareNode : public SignalNode {
public:
    SignalSquareNode() : SignalNode(ECalcNodeType::WAVEFORM){};
    float Result(float x) { return waves.pulse.result(x); }
};
class SignalSawNode : public SignalNode {
public:
    SignalSawNode() : SignalNode(ECalcNodeType::WAVEFORM){};
    float Result(float x) { return waves.saw.result(x); }
};
class SignalNoiseNode : public SignalNode {
public:
    SignalNoiseNode() : SignalNode(ECalcNodeType::WAVEFORM){};
    float Result(float x) { return waves.noise.result(x); }
};

