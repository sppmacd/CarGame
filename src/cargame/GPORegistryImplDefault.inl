// don't include GPORegistry.hpp
// because THIS file is included by GPORegistry.hpp

#include "DebugLogger.hpp"

GPOREGISTRY_TEMPLATE
GPORegistry<IdT, ObjT>::GPORegistry()
{
    greatestId = IdT();
}

GPOREGISTRY_TEMPLATE
GPORegistry<IdT, ObjT>::~GPORegistry()
{
    clear();
}

GPOREGISTRY_TEMPLATE
void GPORegistry<IdT, ObjT>::clear()
{
    for(auto it = objects.begin(); it != objects.end(); it++)
    {
        delete it->second;
    }
    objects.clear();
}

GPOREGISTRY_TEMPLATE
int GPORegistry<IdT, ObjT>::add(IdT id, ObjT* obj)
{
    ObjT* obj2 = findById(id);
    if(obj2)
    {
        return REG_ERROR_EXISTS;
    }
    try
    {
        objects.push_back(std::make_pair(id, obj));
    }
    catch(...)
    {
        return REG_ERROR_UNKNOWN;
    }
    if(greatestId < id)
        greatestId = id;
    return 0;
}

GPOREGISTRY_TEMPLATE
IdT GPORegistry<IdT, ObjT>::add(ObjT* obj)
{
    /* generate a new ID*/
    IdT id = greatestId + 1;

    /* add object an return its ID*/
    return (add(id, obj) == 0) ? id : 0;
}

GPOREGISTRY_TEMPLATE
ObjT* GPORegistry<IdT, ObjT>::findById(const IdT& id) const
{
    for(auto it = objects.begin(); it != objects.end(); it++)
    {
        if(it->first == id)
            return it->second;
    }
    return NULL;
}

GPOREGISTRY_TEMPLATE
IdT GPORegistry<IdT, ObjT>::getIdOf(ObjT* obj) const
{
    for(auto it = objects.begin(); it != objects.end(); it++)
    {
        if(it->second == obj)
            return it->first;
    }
    return IdT();
}

GPOREGISTRY_TEMPLATE
void GPORegistry<IdT, ObjT>::remove(const IdT& id)
{
    for(auto it = objects.begin(); it != objects.end(); it++)
    {
        if(it->first == id)
        {
            delete it->second;
            objects.erase(it);
            return;
        }
    }
}

GPOREGISTRY_TEMPLATE
void GPORegistry<IdT, ObjT>::remove(ObjT* obj)
{
    for(auto it = objects.begin(); it != objects.end(); it++)
    {
        if(it->second == obj)
        {
            delete it->second;
            objects.erase(it);
            return;
        }
    }
}

GPOREGISTRY_TEMPLATE
int GPORegistry<IdT, ObjT>::count() const
{
    return objects.size();
}

GPOREGISTRY_TEMPLATE
const typename GPORegistry<IdT, ObjT>::ArrayType& GPORegistry<IdT, ObjT>::arr() const
{
    return objects;
}

// VOID

// don't deallocate void objects
template<class IdT>
void GPORegistry<IdT, void>::clear()
{
    objects.clear();
}
template<class IdT>
void GPORegistry<IdT, void>::remove(const IdT& id)
{
    for(auto it = objects.begin(); it != objects.end(); it++)
    {
        if(it->first == id)
        {
            objects.erase(it);
            return;
        }
    }
}
template<class IdT>
void GPORegistry<IdT, void>::remove(void* obj)
{
    for(auto it = objects.begin(); it != objects.end(); it++)
    {
        if(it->second == obj)
        {
            objects.erase(it);
            return;
        }
    }
}
