//
//  SignalNodes.cpp
//  example
//
//  Created by Joshua Batty on 21/9/17.
//
//

#include "SignalNodes.h"


//--------------------------------------------------------------
void SignalNode::update() {
    gtf::NodeConnectionF32 * output = gtf::NodeConnectionF32::CAST(outputConnections[0]);
    
    number = output->data = ofMap(Result(ofGetElapsedTimef()*speed),-1.0,1.0,amp_min,amp_max);
    output->isReady = true;
}
