//
//  CalcNodes.cpp
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "CalcNodes.h"

//--------------------------------------------------------------
void OutputNode::update() {
    gtf::NodeConnectionF32 * input = gtf::NodeConnectionF32::CAST(inputConnections[0]);
        
    if(inputConnections[0]->input != nullptr &&
       inputConnections[0]->input->isReady) {
        gtf::NodeConnectionF32 const * inputSource = gtf::NodeConnectionF32::CAST(inputConnections[0]->input);
        number = input->data = inputSource->data;
    }
    else {
        number = input->data = 0;
    }
}

//--------------------------------------------------------------
void CalcWaveNode::update() {
    gtf::NodeConnectionF32 * output = gtf::NodeConnectionF32::CAST(outputConnections[0]);
    
    number = output->data = ofMap(Result(ofGetElapsedTimef()*speed),-1.0,1.0,amp_min,amp_max);
    output->isReady = true;
}

//--------------------------------------------------------------
void CalcNumberNode::update() {
    if(dirty){
        
        for (gtf::NodeConnectionBase* con : outputConnections) {
            gtf::NodeConnectionF32 * numberCon = gtf::NodeConnectionF32::CAST(con);
            numberCon->data = number;
        }
        
        for (gtf::NodeConnectionBase* con : outputConnections[0]->output) {
            gtf::NodeConnectionF32 * numberCon = gtf::NodeConnectionF32::CAST(con);
            numberCon->isDirty = true;
        }
        
        outputConnections[0]->isReady = true;
        dirty = false;
    }
}

//--------------------------------------------------------------
void CalcMathOpNode::update() {
    if(inputConnections[0]->isDirty || inputConnections[1]->isDirty) {
        dirty = true;
    }
    
    // Set To True so everything is calculated each frame
    // Comment out so nodes are only calculated when nodes are connected
    dirty = true;

    if(dirty) {
        gtf::NodeConnectionF32 * inputA = gtf::NodeConnectionF32::CAST(inputConnections[0]);
        gtf::NodeConnectionF32 * inputB = gtf::NodeConnectionF32::CAST(inputConnections[1]);
        
        int readyInputs = 0;
        
        if(inputConnections[0]->input != nullptr &&
           inputConnections[0]->input->isReady) {
            gtf::NodeConnectionF32 const * inputASource = gtf::NodeConnectionF32::CAST(inputConnections[0]->input);
            inputA->data = inputASource->data;
            readyInputs++;
        }
        else {
            inputA->data = 0;
        }
        
        if(inputConnections[1]->input != nullptr &&
           inputConnections[1]->input->isReady) {
            gtf::NodeConnectionF32 const * inputBSource = gtf::NodeConnectionF32::CAST(inputConnections[1]->input);
            inputB->data = inputBSource->data;
            readyInputs++;
        }
        else {
            inputB->data = 0;
        }
        
        {
            inputA->isDirty = false;
            inputB->isDirty = false;
            gtf::NodeConnectionF32 * output = gtf::NodeConnectionF32::CAST(outputConnections[0]);
            
            number = output->data = MathOp(inputA->data, inputB->data);
            output->isReady = true; //(readyInputs == 2);
            
            for(auto otherOut : output->output)
            {
                otherOut->isDirty = true;
            }
            
            dirty = false;
        }
    }
}
