#include "LayerStack.h"

namespace Hazel
{
    LayerStack::LayerStack() {
    
        m_LayerInsert = m_Layers.begin();
    }

    void LayerStack::PushLayer(Layer *layer)
    {
        m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
    }

    LayerStack::~LayerStack()
    {
        for (Layer *layer : m_Layers)
        {
            delete layer;
        }
    }

    void LayerStack::PushOverlay(Layer *overlay)
    {
        m_Layers.emplace_back(overlay);
    }

/**
 * 从图层栈中移除指定的图层
 * @param layer 要移除的图层指针
 */
    void LayerStack::PopLayer(Layer *layer)
    {
        auto it = std::find(m_Layers.begin(),m_Layers.end(),layer);

        if( it != m_Layers.end()){

            m_Layers.erase(it);
            m_LayerInsert--;
        }
    }


    void LayerStack::PopOverlay(Layer* overlay){
         auto it = std::find(m_Layers.begin(),m_Layers.end(),overlay);
         
         if(it!=m_Layers.end()){
            m_Layers.erase(it);
         }
    }

}