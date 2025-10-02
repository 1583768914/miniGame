namespace Hazel
{

    void LayerStack::PushLayer(Layer *layer)
    {
        m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
    }

    // 构造函数，初始化LayerStack对象
    LayerStack::LayerStack()
    {
        // 将m_LayerInsert初始化为m_Layers容器的起始位置
        m_LayerInsert = m_Layers.begin();
    }

    LayerStack::~LayerStack()
    {
        for (Layer *layer : m_Layers)
        {
            delete layer;
        }
    }

    /**
     * 将一个新层(Layer)推入到层堆栈(LayerStack)中
     * @param layer 指向要添加的层的指针
     */
    void LayerStack::PushLayer(Layer *layer)
    {
        m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
    }

    void LayerStack::PushOverlay(Layer *overlay)
    {
        m_Layers.emplace_back(overlay);
    }

/**
 * 从图层栈中移除指定的图层
 * @param layer 要移除的图层指针
 */
    void LayerStacl::PopLayer(Layer *layer)
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