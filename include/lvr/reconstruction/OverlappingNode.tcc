namespace lvr{

template<typename VertexT>
OverlappingNode<VertexT>::OverlappingNode(unsigned int bucket_size)
:m_split_value(0),
m_split_dim(0),
m_bucket_size(bucket_size),
m_points(boost::shared_ptr<std::vector<VertexT> >(new std::vector<VertexT>() ) )
{
    
}

// Node Functions

template<typename VertexT>
bool OverlappingNode<VertexT>::isLeaf()
{
    return (!m_left || !m_right);
}

template<typename VertexT>
boost::shared_ptr<OverlappingNode<VertexT> > OverlappingNode<VertexT>::leftChild()
{
    return m_left;
}

template<typename VertexT>
void OverlappingNode<VertexT>::setLeftChild(OverlappingNode<VertexT>* node)
{
    boost::shared_ptr<OverlappingNode<VertexT> > node_ptr(node);
    setLeftChild(node_ptr);
}

template<typename VertexT>
void OverlappingNode<VertexT>::setLeftChild(boost::shared_ptr<OverlappingNode<VertexT> > node)
{
    m_left = node;
}

template<typename VertexT>
boost::shared_ptr<OverlappingNode<VertexT> > OverlappingNode<VertexT>::rightChild()
{
    return m_right;
}

template<typename VertexT>
void OverlappingNode<VertexT>::setRightChild(OverlappingNode<VertexT>* node)
{
    boost::shared_ptr<OverlappingNode<VertexT> > node_ptr(node);
    setRightChild(node_ptr);
}

template<typename VertexT>
void OverlappingNode<VertexT>::setRightChild(boost::shared_ptr<OverlappingNode<VertexT> > node)
{
    m_right = node;
}

template<typename VertexT>
OverlappingNode<VertexT>* OverlappingNode<VertexT>::getParent()
{
    return m_parent;
}

template<typename VertexT>
void OverlappingNode<VertexT>::setParent(OverlappingNode<VertexT>* node)
{
    
    m_parent = node;
}

// template<typename VertexT>
// void OverlappingNode<VertexT>::setParent(boost::shared_ptr<OverlappingNode<VertexT> > node)
// {
//     m_parent = node;
// }

template<typename VertexT>
double OverlappingNode<VertexT>::getSplitValue()
{
    return m_split_value;
}

template<typename VertexT>
unsigned int OverlappingNode<VertexT>::getSplitDimension()
{
    return m_split_dim;
}

// Leaf Functions

template<typename VertexT>
std::vector<VertexT>& OverlappingNode<VertexT>::getOverlap(unsigned int lap_id)
{
    if( m_overlaps.size() > lap_id )
    {
        return m_overlaps[lap_id];
    }
}

template<typename VertexT>
void OverlappingNode<VertexT>::calculateOverlap(double overlap_size)
{
    if(!isLeaf())
    {
        this->m_left->calculateOverlap(overlap_size);
        this->m_right->calculateOverlap(overlap_size);
    }else{
        // Resize to 6 overlap volumes
        // 0: dim0 minimum, 1:dim0 maximum
        m_overlaps.resize(6);
        VertexT min = m_bounding_box.getMin();
        VertexT max = m_bounding_box.getMax();      



        for(auto it = m_points->begin(); it != m_points->end(); ++it)
        {
            // x dim check
            if( (*it)[0] - min[0] <= overlap_size )
            {
                // is in low volume
                m_overlaps[0].push_back(*it);
            }else if( max[0] - (*it)[0] <= overlap_size )
            {
                // is in high volume
                m_overlaps[1].push_back(*it);
            }

            // y dim check
            if( (*it)[1] - min[1] <= overlap_size )
            {
                // is in low volume
                m_overlaps[2].push_back(*it);
            }else if( max[1] - (*it)[1] <= overlap_size )
            {
                // is in high volume
                m_overlaps[3].push_back(*it);
            }

            if( (*it)[2] - min[2] <= overlap_size )
            {
                // is in low volume
                m_overlaps[4].push_back(*it);
            }else if( max[2] - (*it)[2] <= overlap_size )
            {
                // is in high volume
                m_overlaps[5].push_back(*it);
            }
        }
    }
}

template<typename VertexT>
unsigned int OverlappingNode<VertexT>::getLongestSideDim()
{
    unsigned int res = 0;
    float longest_dim = m_bounding_box.getXSize();
    
    if(m_bounding_box.getYSize() > longest_dim)
    {
        res = 1;
        longest_dim = m_bounding_box.getYSize();
    }

    if(m_bounding_box.getZSize() > longest_dim)
    {
        res = 2;
    }

    return res;
}

template<typename VertexT>
void OverlappingNode<VertexT>::split()
{
    std::cout << "We have to SPLIT" << std::endl;
    // point size too big
    // split points along longest bb axis with centroid of bounding box
    m_split_dim = this->getLongestSideDim();
    m_split_value = (m_bounding_box.getCentroid())[m_split_dim];

    // create left and right child
    m_left = boost::shared_ptr< OverlappingNode<VertexT> >(new OverlappingNode<VertexT>(m_bucket_size) );
    // m_left->setParent(this);
    m_right = boost::shared_ptr< OverlappingNode<VertexT> >(new OverlappingNode<VertexT>(m_bucket_size) );
    // m_right->setParent(this);


    // split/copy points
    unsigned int num_left = 0;
    unsigned int num_right = 0;

    for(auto iterator = m_points->begin(); iterator != m_points->end(); ++iterator )
    {
        VertexT point = *iterator;
        if( point[m_split_dim] < m_split_value )
        {
            //left
            m_left->insert(point);
            num_left ++;
        } else {
            //right
            m_right->insert(point);
            num_right ++;
        }
    }

    // delete points
    m_points->clear();
    m_points.reset();

    // set parent
    m_left->setParent(this);
    m_right->setParent(this);

}

template<typename VertexT>
void OverlappingNode<VertexT>::insert(VertexT point)
{
    // No Leaf node
    if(!isLeaf())
    {
        if(point[m_split_dim] < m_split_value)
        {
            this->m_left->insert(point);
        }else{
            this->m_right->insert(point);
        }
    }
    else // Leaf Node
    {
        m_points->push_back(point);
        // update next split information like mean value and 
        m_bounding_box.expand(point);

        if(m_points->size() > m_bucket_size)
        {
            this->split();
        }
    }
    
}

template<typename VertexT>
OverlappingNode<VertexT>* OverlappingNode<VertexT>::getNeighbour()
{
    if( this->getParent()->leftChild().get() == this )
    {
        return this->getParent()->rightChild().get();
    }
    else
    {
        return this->getParent()->leftChild().get();
    }
}


template<typename VertexT>
unsigned int OverlappingNode<VertexT>::getLeafSize()
{
    return this->m_points->size();
}

template<typename VertexT>
boost::shared_ptr< std::vector<VertexT> > OverlappingNode<VertexT>::getPoints()
{
    return m_points;
}

template<typename VertexT>
std::vector< std::vector<VertexT> > OverlappingNode<VertexT>::getNeighboursOverlap()
{
    std::vector< std::vector<VertexT> > dest;
    dest.resize(0);
    OverlappingNode<VertexT>* it = this;

    if(it->getParent())
    {
        OverlappingNode<VertexT>* parent = it->getParent();
        OverlappingNode<VertexT>* neigbour = it->getNeighbour();
        
        unsigned int split_dim = parent->getSplitDim();
        
        if(split_dim == 0)
        {
            
        }
    }

    // is there a neighbour in first dim?
    // min neighbors this node min x = neighbor node max x

    return dest;
}


}//namespace lvr