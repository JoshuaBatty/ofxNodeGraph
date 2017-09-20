//
//  NodeGraph.cpp
//  gtf
//
//  Created by David Gallardo on 31/05/16.
//  Copyright (c) 2016 GTF Development Group. All rights reserved.
//

#include <algorithm>
#include "NodeGraph.h"
#include "ErrorHandling.h"

static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x+rhs.x, lhs.y+rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x-rhs.x, lhs.y-rhs.y); }

namespace gtf 
{
Node::~Node()
{
    for (NodeConnectionBase* con : inputConnections)
    {
        if(con->input != nullptr)
        {
            con->input->output.remove(con);
        }
        
        delete con;
    }
    
    for (NodeConnectionBase* con : outputConnections)
    {
        for (auto out : con->output)
        {
            out->input = nullptr;
            out->isDirty = true;
        }
        
        delete con;
    }
}

void NodeGraphType::registerNodeType(NodeTypeBase* nodeType)
{
    GTF_ASSERT(nodeType != nullptr, "Node type is NULL");
    
    auto nt = nodeTypeRegistry.find(nodeType->unique_id);

    if(nt == nodeTypeRegistry.end())
    {
        nodeTypeRegistry.insert(std::make_pair(nodeType->unique_id, nodeType));
    }
}



Node* NodeGraph::createNode(UniqueId typeId, float atX, float atY)
{
    auto typeIt = graphType->nodeTypeRegistry.find(typeId);
    if(typeIt != graphType->nodeTypeRegistry.end())
    {
        NodeTypeBase* nodeType = typeIt->second;
        Node* node = nodeType->createNodeInstance();
        node->nodeId = nodeCounter++;
        
        ImVec2 inputTextSize(0.0f, 0.0f);
        ImVec2 outputText(0.0f, 0.0f);
        
        ImVec2 titleSize = ImGui::CalcTextSize(node->type->displayName.c_str());
        
        titleSize.y *= 3;
        
        for(NodeConnectionDescBase* desc : nodeType->inputConnectionsDesc)
        {
            auto con = desc->createConnectionInstance(true);
            
            ImVec2 textSize = ImGui::CalcTextSize(desc->displayName.c_str());
            inputTextSize.x = std::max<float>(textSize.x, inputTextSize.x);
            
            
            con->posX = 0;
            con->posY = titleSize.y + inputTextSize.y + (textSize.y / 2.0f);
            
            inputTextSize.y += textSize.y;
            inputTextSize.y += 4.0f;		// size between text entries
            
            con->parentNode = node;
            node->inputConnections.push_back(con);
        }
        
        inputTextSize.x += 40.0f;
        float xStart = inputTextSize.x;
        
        float textSizeYOutputs = 0;
        for (NodeConnectionDescBase* desc : nodeType->outputConnectionsDesc)
        {
            ImVec2 textSize = ImGui::CalcTextSize(desc->displayName.c_str());
            inputTextSize.x = std::max<float>(xStart + textSize.x, inputTextSize.x);
            textSizeYOutputs += textSize.y + 4.0f;
        }
        
        inputTextSize.y = std::max(inputTextSize.y, textSizeYOutputs);
        
        node->width = inputTextSize.x;
        node->height = inputTextSize.y + titleSize.y + 4.0f;
        
        inputTextSize.y = 0.0f;
        
        for(NodeConnectionDescBase* desc : nodeType->outputConnectionsDesc)
        {
            auto con = desc->createConnectionInstance(false);
            
            ImVec2 textSize = ImGui::CalcTextSize(desc->displayName.c_str());
            
            con->posX = node->width;
            con->posY = titleSize.y + inputTextSize.y + textSize.y / 2.0f;
            
            inputTextSize.y += textSize.y;
            inputTextSize.y += 4.0f;		// size between text entries
            
            con->parentNode = node;
            node->outputConnections.push_back(con);
        }
        
        node->posX = atX;
        node->posY = atY;
        
        //node->width = 80;
        //node->height = 40 + glm::max(40.0f, node->inputConnections.size() * 20.0f);
        
        nodeList.push_back(node);
        return node;
    }
    
    return nullptr;
}

void NodeGraph::updateNodes()
{
    for (Node* node : nodeList)
    {
        node->update();
    }
}

NodeGraph::~NodeGraph()
{
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void NodeGraph::drawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2, int STEPS = 32)
{
    ImVec2 diff = p2 - p1;
    
    if(diff.x < 0)
    {
        diff.x = abs(diff.x) * 3;
        if(diff.x > 800)
            STEPS = 64;
    }
    else
    {
        diff.x = 0;
    }
    
	ImVec2 t1 = ImVec2(+180.0f + diff.x + abs(diff.y*1.5f), 0.0f);
	ImVec2 t2 = ImVec2(+180.0f + diff.x + abs(diff.y*1.5f), 0.0f);
    
	for (int step = 0; step <= STEPS; step++)
	{
		float t = (float)step / (float)STEPS;
		float h1 = +2*t*t*t - 3*t*t + 1.0f;
		float h2 = -2*t*t*t + 3*t*t;
		float h3 =    t*t*t - 2*t*t + t;
		float h4 =    t*t*t -   t*t;
		drawList->PathLineTo(ImVec2(h1*p1.x + h2*p2.x + h3*t1.x + h4*t2.x, h1*p1.y + h2*p2.y + h3*t1.y + h4*t2.y));
	}
    
	drawList->PathStroke(ImColor(200,200,100), false, 3.0f);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool NodeGraph::isConnectorHovered(NodeConnectionBase* c, ImVec2 offset)
{
	ImVec2 mousePos = ImGui::GetIO().MousePos;
	ImVec2 conPos = offset + ImVec2(c->posX, c->posY);
    
	float xd = mousePos.x - conPos.x;
	float yd = mousePos.y - conPos.y;
    
	return ((xd * xd) + (yd *yd)) < (NodeGRAPH::GTF_NODE_SLOT_RADIUS * NodeGRAPH::GTF_NODE_SLOT_RADIUS);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

NodeConnectionBase* NodeGraph::getHoverCon(ImVec2 offset, ImVec2* pos, bool resetCon)
{
	for (Node* node : nodeList)
	{
		ImVec2 nodePos = ImVec2(node->posX, node->posY) + offset;
        
		for (NodeConnectionBase* con : node->inputConnections)
		{
			if (isConnectorHovered(con, nodePos))
			{
				*pos = nodePos + ImVec2(con->posX, con->posY);
				return con;
			}
		}
        
		for (NodeConnectionBase* con : node->outputConnections)
		{
			if (isConnectorHovered(con, nodePos))
			{
				*pos = nodePos + ImVec2(con->posX, con->posY);
				return con;
			}
		}
	}
	
    if(resetCon)
    {
        dragNode.con = 0;
    }
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool checkRecursiveIntegrity(NodeConnectionBase const * targetCon, Node const * forbbidenNode)
{
    if(!targetCon) return true;
    if(targetCon->parentNode == forbbidenNode) return false;
    
    if(targetCon->isInput)
    {
        for(auto outCon : targetCon->parentNode->outputConnections)
        {
            for(auto con : outCon->output)
            {
                if(!checkRecursiveIntegrity(con, forbbidenNode))
                {
                    return false;
                }
            }
        }
    }
    else
    {
        for(auto inCon : targetCon->parentNode->inputConnections)
        {
            if(!checkRecursiveIntegrity(inCon->input, forbbidenNode))
            {
                return false;
            }
        }
    }
    
    return true;
}

void NodeGraph::updateDraging(ImVec2 offset)
{
	switch (dragState)
	{
		case GTFDragState::DS_DEFAULT:
		{
			ImVec2 pos;
			NodeConnectionBase* con = getHoverCon(offset, &pos);
            
			if (con)
			{
				dragNode.con = con;
				dragNode.pos = pos;
				dragState = GTFDragState::DS_HOVER;
				return;
			}
            
			break;
		}
            
		case GTFDragState::DS_HOVER:
		{
			ImVec2 pos;
			NodeConnectionBase* con = getHoverCon(offset, &pos);
            
			// Make sure we are still hovering the same node
            
			if (con != dragNode.con)
			{
				dragNode.con = 0;
				dragState = GTFDragState::DS_DEFAULT;
				return;
			}
            
            bool ctrlPressed = ImGui::GetIO().KeyCtrl;
            
			if (ImGui::IsMouseClicked(0) && dragNode.con && !ctrlPressed)
            {
				dragState = GTFDragState::DS_DRAGING;
                return;
            }
            
            if (ImGui::IsMouseClicked(0) && dragNode.con && ctrlPressed)
            {
                if(dragNode.con->isInput && dragNode.con->input)
                {
                    dragNode.con->input->output.remove(dragNode.con);
                    dragNode.con->input = nullptr;
                    dragNode.con->isDirty = true;
                }
                else
                {
                    for(auto outputCon : dragNode.con->output)
                    {
                        outputCon->isDirty = true;
                        outputCon->input = nullptr;
                    }
                    
                    dragNode.con->output.clear();
                }
            }
                
            
			break;
		}
            
		case GTFDragState::DS_BEGIN_DRAG:
		{
			break;
		}
            
		case GTFDragState::DS_DRAGING:
		{
    		ImDrawList* drawList = ImGui::GetWindowDrawList();
            
			drawList->ChannelsSetCurrent(0); // Background
            
            
            if(dragNode.con->isInput)
            {
                drawHermite(drawList, ImGui::GetIO().MousePos, dragNode.pos);
            }
            else
            {
                drawHermite(drawList, dragNode.pos, ImGui::GetIO().MousePos);
            }
            
			if (!ImGui::IsMouseDown(0))
			{
				ImVec2 pos;
				NodeConnectionBase* con = getHoverCon(offset, &pos);
                
				// Make sure we are still hovering the same node
                
				if (con == dragNode.con)
				{
					dragNode.con = 0;
					dragState = GTFDragState::DS_DEFAULT;
					return;
				}
                
				// Lets connect the nodes.
				// TODO: Make sure we connect stuff in the correct way!
                // TODO2: fixed at node type level, add rules for custom connections?
                // TODO3: prevent recursive node graphs (DONE)
                if(con->isInput != dragNode.con->isInput && checkRecursiveIntegrity(con, dragNode.con->parentNode))
                {
                    if(con->isInput)
                    {
                        con->input = dragNode.con;
                        con->isDirty = true;
                        dragNode.con->output.push_back(con);
                    }
                    else
                    {
                        con->output.push_back(dragNode.con);
                        dragNode.con->input = con;
                        dragNode.con->isDirty = true;
                    }
                    
                    
                }
                
                dragNode.con = 0;
                dragState = GTFDragState::DS_DEFAULT;
			}
            
			break;
		}
            
		case GTFDragState::DS_CONNCECT:
		{
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: Ugly fix: me

Node* NodeGraph::findNodeByCon(NodeConnectionBase const * findCon)
{
	for (Node* node : nodeList)
	{
		for (NodeConnectionBase* con : node->inputConnections)
		{
			if (con == findCon)
				return node;
		}
        
		for (NodeConnectionBase* con : node->outputConnections)
		{
			if (con == findCon)
				return node;
		}
	}
    
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void NodeGraph::renderLines(ImDrawList* drawList, ImVec2 offset)
{
	for (Node* node : nodeList)
	{
		for (NodeConnectionBase* con : node->inputConnections)
		{
			if (!con->input)
				continue;
            
			Node* targetNode = findNodeByCon(con->input);
            
			if (!targetNode)
				continue;
            
			drawHermite(drawList,
                        offset + ImVec2(targetNode->posX, targetNode->posY) + ImVec2(con->input->posX, con->input->posY),
                        offset + ImVec2(node->posX, node->posY) + ImVec2(con->posX, con->posY));
		}
	}
}

void NodeGraph::displayNode(ImDrawList* drawList, ImVec2 offset, Node* node, int& node_selected)
{
    int node_hovered_in_scene = -1;
    bool open_context_menu = false;
    
	ImGui::PushID(node->nodeId);
	ImVec2 node_rect_min = offset + ImVec2(node->posX, node->posY);
    
	// Display node contents first
	//drawList->ChannelsSetCurrent(1); // Foreground
    
    ImGui::SetCursorScreenPos(node_rect_min);
	ImGui::InvisibleButton("node", ImVec2(node->width, node->height));
    
	if (ImGui::IsItemHovered())
	{
		node_hovered_in_scene = node->nodeId;
		open_context_menu |= ImGui::IsMouseClicked(1);
        
        if(ImGui::IsMouseClicked(0))
        {
            node->mouseClicking = true;
            lastClickedNode = node;
            
            if(ImGui::GetIO().KeyShift)
            {
                
                if(std::find(selectedNodes.begin(), selectedNodes.end(), node) == selectedNodes.end())
                {
                    node->selected = true;
                    selectedNodes.push_back(node);
                }
                else
                {
                    node->selected = false;
                    selectedNodes.remove(node);
                }
            }
            else
            {
                if(std::find(selectedNodes.begin(), selectedNodes.end(), node) == selectedNodes.end())
                {
                    for (Node* n : selectedNodes)
                    {
                        n->selected = false;
                    }
                    selectedNodes.clear();
                    node->selected = true;
                    selectedNodes.push_back(node);
                }
            }
        }
        /*else if(ImGui::IsMouseReleased(0))
        {
            if(node->mouseClicking)
            {
                
            }
        }*/
    }
    
    bool node_moving_active = false;
	if (ImGui::IsItemActive() && !dragNode.con && node->selected)
		node_moving_active = true;
    
    // Draw text bg area
    ImVec2 node_rect_max = node_rect_min + ImVec2(node->width, node->height);
    ImVec2 titleArea = node_rect_max;
	titleArea.y = node_rect_min.y + 30.0f;
    ImU32 node_bg_color = (node_hovered_in_scene == node->nodeId) ? ImColor(75,75,75) : ImColor(60,60,60);
    
    if(node->selected)
    {
        drawList->AddRect(node_rect_min - NodeGRAPH::GTF_NODE_SSELECTED_MARGIN,
                          node_rect_max + NodeGRAPH::GTF_NODE_SSELECTED_MARGIN, ImColor(100,100,100), 4.0f);
    }
    
	drawList->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
	drawList->AddRectFilled(node_rect_min + ImVec2(1,1), titleArea, ImColor(100,0,0), 4.0f);
	drawList->AddRect(node_rect_min, node_rect_max, ImColor(100,100,100), 4.0f);
    
	bool old_any_active = ImGui::IsAnyItemActive();
    
	// Draw title in center
    
	ImVec2 textSize = ImGui::CalcTextSize(node->type->displayName.c_str());
    
	ImVec2 pos = node_rect_min + NodeGRAPH::GTF_NODE_WINDOW_PADDING;
	pos.x = node_rect_min.x + (node->width / 2) - textSize.x / 2;
    
	ImGui::SetCursorScreenPos(pos);
	//ImGui::BeginGroup(); // Lock horizontal position
	ImGui::Text("%s", node->type->displayName.c_str());
    
	// Save the size of what we have emitted and weither any of the widgets are being used
	bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
	//node->size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
	
    
	// Display node box
	//drawList->ChannelsSetCurrent(0); // Background
    
	offset.y += 40.0f;
    
	offset = node_rect_min;
    
    ImVec2 textOffset;
	textOffset.x = node_rect_min.x;
	textOffset.y = node_rect_min.y;
    textOffset.y += 40.0f;
    
	for (NodeConnectionBase* con : node->inputConnections)
	{
		ImGui::SetCursorScreenPos(textOffset + ImVec2(10.0f, 0));
        
        auto numberCon = NodeConnectionF32::CAST(con);
        
        if(numberCon)
        {
            ImGui::Text("%.1f", numberCon->data);
            //ImGui::Text("%d", numberCon->data);
        }
        else
        {
            ImGui::Text("%s", con->desc->displayName.c_str());
        }
        
        
		ImColor conColor = ImColor(150, 150, 150);
        
		if (isConnectorHovered(con, node_rect_min))
			conColor = ImColor(200, 200, 200);
        
		drawList->AddCircleFilled(node_rect_min + ImVec2(con->posX, con->posY), NodeGRAPH::GTF_NODE_SLOT_RADIUS, conColor);
        
		textOffset.y += textSize.y + 2.0f;
	}
    
	textOffset = node_rect_min;
	textOffset.y += 40.0f;
    
	for (NodeConnectionBase* con : node->outputConnections)
	{
		textSize = ImGui::CalcTextSize(con->desc->displayName.c_str());
        
		ImGui::SetCursorScreenPos(textOffset + ImVec2(con->posX - (textSize.x + 10.0f), 0));
        
        auto numberCon = NodeConnectionF32::CAST(con);

        if(numberCon)
        {
            ImGui::Text("%.1f", numberCon->data);
            //ImGui::Text("%d", numberCon->data);
        }
        else
        {
            //cout << "dddds" << endl;
            ImGui::Text("%s", con->desc->displayName.c_str());
        }
        
        
		ImColor conColor = ImColor(150, 150, 150);
        
		if (isConnectorHovered(con, node_rect_min))
			conColor = ImColor(200, 200, 200);
        
		drawList->AddCircleFilled(node_rect_min + ImVec2(con->posX, con->posY), NodeGRAPH::GTF_NODE_SLOT_RADIUS, conColor);
        
		textOffset.y += textSize.y + 2.0f;
	}
    
    
	//for (int i = 0; i < node->outputConnections.size(); ++i)
	//	drawList->AddCircleFilled(offset + node->outputSlotPos(i), NODE_SLOT_RADIUS, ImColor(150,150,150,150));
    
	if (node_widgets_active || node_moving_active)
		node_selected = node->nodeId;
    
	if (node_moving_active && ImGui::IsMouseDragging(0))
    {
		//node->posX += ImGui::GetIO().MouseDelta.x;
        //node->posY += ImGui::GetIO().MouseDelta.y;
        for (Node* n : selectedNodes)
        {
            n->posX += ImGui::GetIO().MouseDelta.x;
            n->posY += ImGui::GetIO().MouseDelta.y;
        }
    }
    
	//ImGui::EndGroup();
    
	ImGui::PopID();
}


void NodeGraph::updateGUI()
{
    bool open_context_menu = false;
    int node_hovered_in_list = -1;
    int node_hovered_in_scene = -1;
    
	static int node_selected = -1;
    static ImVec2 scrolling = ImVec2(0.0f, 0.0f);
    
    ImGui::SameLine();
    ImGui::BeginGroup();
    
    // Create our child canvas
    //ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1,1));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0,0));
    ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImColor(40,40,40,200));
    ImGui::BeginChild("scrolling_region", ImVec2(0,0), true, ImGuiWindowFlags_NoScrollbar|ImGuiWindowFlags_NoMove);
    ImGui::PushItemWidth(120.0f);
    
    
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->ChannelsSplit(2);
    //ImVec2 offset = ImGui::GetCursorScreenPos() - scrolling;
    
	//displayNode(draw_list, scrolling, s_emittable, node_selected);
	//displayNode(draw_list, scrolling, s_emitter, node_selected);
    
    ImVec2 curOffset = ImGui::GetCursorScreenPos();
    
    if(lastClickedNode != nullptr && !ImGui::IsMouseClicked(0) && nodeList.front() != lastClickedNode )
    {
        nodeList.remove(lastClickedNode);
        nodeList.push_back(lastClickedNode);
    }
    
	for (Node* node : nodeList)
		displayNode(draw_list, curOffset - scrolling , node, node_selected);
    
	updateDraging(curOffset - scrolling);
	renderLines(draw_list, curOffset - scrolling);
    
    draw_list->ChannelsMerge();
    
    ImVec2 hoveredConPos;
    NodeConnectionBase* hoveredCon = getHoverCon(curOffset - scrolling, &hoveredConPos, false);
    
    // Open context menu
    if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow())
    {
        if(ImGui::IsMouseClicked(1))
        {
            node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
            open_context_menu = true;
        }
        else if(ImGui::IsMouseClicked(0) && hoveredCon == nullptr)
        {
            for (Node* n : selectedNodes)
            {
                n->selected = false;
            }
            selectedNodes.clear();
        }
    }
    
    if(ImGui::IsKeyDown(ImGui::GetKeyIndex(ImGuiKey_Delete)))
    {
        lastClickedNode = nullptr;
        for(Node* node : selectedNodes)
        {
            nodeList.remove(node);
            delete node;
        }
        
        selectedNodes.clear();
    }

    if (open_context_menu)
    {
        ImGui::OpenPopup("context_menu");
        if (node_hovered_in_list != -1)
            node_selected = node_hovered_in_list;
        if (node_hovered_in_scene != -1)
            node_selected = node_hovered_in_scene;
    }
    
    // Draw context menu
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8,8));
    if (ImGui::BeginPopup("context_menu"))
    {
        for (auto nodeTypeIt : this->graphType->nodeTypeRegistry)
        {
            if (ImGui::MenuItem(nodeTypeIt.second->displayName.c_str()))
			{
                ImVec2 nodePos = ImGui::GetIO().MousePos - curOffset + scrolling;
                createNode(nodeTypeIt.second->unique_id, nodePos.x, nodePos.y);
            }
        }
        
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    
    // Scrolling
    if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
        scrolling = scrolling - ImGui::GetIO().MouseDelta;
    
    ImGui::PopItemWidth();
    ImGui::EndChild();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::EndGroup();
    
    //ImGui::End();
}

} //namespace gtf
