#pragma once

#include "Core/Layer.h"

#include <vector> // TArray wasn't designed for use cases like in the FLayerStack, vector will do though.

class FLayerStack
{
public:
	FLayerStack();
	~FLayerStack();

	void PushLayer(FLayer* Layer);
	void PushOverlay(FLayer* Overlay);

	void PopLayer(FLayer* Layer);
	void PopOverlay(FLayer* Overlay);

	std::vector<FLayer*>::iterator begin() { return Layers.begin(); }
	std::vector<FLayer*>::iterator end() { return Layers.end(); }
	std::vector<FLayer*>::reverse_iterator rbegin() { return Layers.rbegin(); }
	std::vector<FLayer*>::reverse_iterator rend() { return Layers.rend(); }

	std::vector<FLayer*>::const_iterator begin() const { return Layers.begin(); }
	std::vector<FLayer*>::const_iterator end() const { return Layers.end(); }
	std::vector<FLayer*>::const_iterator cbegin() const { return  Layers.cbegin(); }
	std::vector<FLayer*>::const_iterator cend() const { return Layers.cend(); }

	std::vector<FLayer*>::const_reverse_iterator rbegin() const { return Layers.rbegin(); }
	std::vector<FLayer*>::const_reverse_iterator rend() const { return Layers.rend(); }
	std::vector<FLayer*>::const_reverse_iterator crbegin() const { return Layers.crbegin(); }
	std::vector<FLayer*>::const_reverse_iterator crend() const { return Layers.crend(); }
private:
	std::vector<FLayer*> Layers;
	uint_t LayerInsertPos = 0;
};
