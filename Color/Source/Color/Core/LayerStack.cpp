#include "ColorPCH.h"
#include "LayerStack.h"

FLayerStack::FLayerStack()
{
}

FLayerStack::~FLayerStack()
{
	for (FLayer* Layer : Layers)
	{
		Layer->OnDetach();
		delete Layer;
	}
}

void FLayerStack::PushLayer(FLayer* Layer)
{
	Layers.emplace(Layers.begin() + LayerInsertPos++, Layer);
}

void FLayerStack::PushOverlay(FLayer* Overlay)
{
	Layers.emplace_back(Overlay);
}

void FLayerStack::PopLayer(FLayer* Layer)
{
	auto It = std::find(Layers.begin(), Layers.begin() + LayerInsertPos, Layer);
	if (It != Layers.begin() + LayerInsertPos--)
	{
		Layer->OnDetach();
		Layers.erase(It);
	}
}

void FLayerStack::PopOverlay(FLayer* Overlay)
{
	auto It = std::find(Layers.begin() + LayerInsertPos, Layers.end(), Overlay);
	if (It != Layers.end())
	{
		Overlay->OnDetach();
		Layers.erase(It);
	}
}
