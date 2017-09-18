//
//  CalcNodes.cpp
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include "CalcNodes.h"

void CalcNumberNode::update()
{
    if(dirty)
    {
        
        for (gtf::NodeConnectionBase* con : outputConnections)
        {
            gtf::NodeConnectionI32 * numberCon = gtf::NodeConnectionI32::CAST(con);
            numberCon->data = number;
        }
        
        for (gtf::NodeConnectionBase* con : outputConnections[0]->output)
        {
            gtf::NodeConnectionI32 * numberCon = gtf::NodeConnectionI32::CAST(con);
            numberCon->isDirty = true;
        }
        
        outputConnections[0]->isReady = true;
        dirty = false;
    }
}

void CalcMathOpNode::update()
{
    if(inputConnections[0]->isDirty || inputConnections[1]->isDirty)
    {
        dirty = true;
    }
    
    if(dirty)
    {
        gtf::NodeConnectionI32 * inputA = gtf::NodeConnectionI32::CAST(inputConnections[0]);
        gtf::NodeConnectionI32 * inputB = gtf::NodeConnectionI32::CAST(inputConnections[1]);
        
        int readyInputs = 0;
        
        if(inputConnections[0]->input != nullptr &&
           inputConnections[0]->input->isReady)
        {
            gtf::NodeConnectionI32 const * inputASource = gtf::NodeConnectionI32::CAST(inputConnections[0]->input);
            inputA->data = inputASource->data;
            readyInputs++;
        }
        else
        {
            inputA->data = 0;
        }
        
        if(inputConnections[1]->input != nullptr &&
           inputConnections[1]->input->isReady)
        {
            gtf::NodeConnectionI32 const * inputBSource = gtf::NodeConnectionI32::CAST(inputConnections[1]->input);
            inputB->data = inputBSource->data;
            readyInputs++;
        }
        else
        {
            inputB->data = 0;
        }
        
        {
            inputA->isDirty = false;
            inputB->isDirty = false;
            gtf::NodeConnectionI32 * output = gtf::NodeConnectionI32::CAST(outputConnections[0]);
            
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