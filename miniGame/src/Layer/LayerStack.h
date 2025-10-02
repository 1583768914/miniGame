#pragma once

#include "Hazel/Core.h"
#include "Layer.h"

#include <vector>

namespace Hazel{
    
    class HAZEL_API LayerStack
    {

        public:
          LayerStack();
          ~LayerStack();

          void PushLayer(Layer* layer); //// vector在头部添加一个层
          void PushOverlay(Layer* overlay); //// 在vector末尾添加一个覆盖层，在屏幕的最上方的层
          void PopLayer(Layer* layer); //// vector弹出指定层
          void PopOverlay(Layer* overlay); //// vector弹出覆盖层
          
          std::vector<Layer>::iterator begin(){return m_Layer.begin();}
          std::vector<Layer>::iterator end(){ return m_Layer.end();};


          private:
            std::vector<Layer*> m_Layers;
            std::vector<Layer*>::iterator m_LayerInsert;
    }
}