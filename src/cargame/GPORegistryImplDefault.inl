// don't include GPORegistry.hpp
// because THIS file is included by GPORegistry.hpp

#include "DebugLogger.hpp"

GPOREGISTRY_TEMPLATE
GPORegistry<IdT, ObjT>::GPORegistry()
{
    greatestNumericId = 0;
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
int GPORegistry<IdT, ObjT>::add(IdT id, ObjT* obj, size_t numeric)
{
    if(numeric != 0)
    {
        // todo: optimize it
        // Check if object already exists, if so, return error.
        for(auto it = objects.begin(); it != objects.end(); it++)
        {
            if(it->first.baseId == id)
                return REG_ERROR_EXISTS;
            if(it->first.numericId == numeric)
                return REG_ERROR_EXISTS_NUMERIC;
        }

        // Try to add object.
        try
        {
            objects.push_back(std::make_pair(IdTEntry{id,numeric}, obj));
        }
        catch(...)
        {
            return REG_ERROR_UNKNOWN;
        }

        if(greatestNumericId < numeric)
            greatestNumericId = numeric;
        return numeric;
    }
    else
    {
        // Generate ID and add the object if %numeric == 0.
        numeric = greatestNumericId + 1;
        return add(id, obj, numeric);
    }
}

GPOREGISTRY_TEMPLATE
ObjT* GPORegistry<IdT, ObjT>::findById(const IdT& id) const
{
    for(auto it = objects.begin(); it != objects.end(); it++)
    {
        if(it->first.baseId == id)
            return it->second;
    }
    return NULL;
}

GPOREGISTRY_TEMPLATE
ObjT* GPORegistry<IdT, ObjT>::findByNumericId(const size_t id) const
{
    for(auto it = objects.begin(); it != objects.end(); it++)
    {
        if(it->first.numericId == id)
            return it->second;
    }
    return NULL;
}

GPOREGISTRY_TEMPLATE
typename GPORegistry<IdT, ObjT>::IdTEntry GPORegistry<IdT, ObjT>::getIdOf(ObjT* obj) const
{
    for(auto it = objects.begin(); it != objects.end(); it++)
    {
        if(it->second == obj)
            return it->first;
    }
    return IdTEntry{IdT(), 0};
}

GPOREGISTRY_TEMPLATE
void GPORegistry<IdT, ObjT>::remove(const IdT& id)
{
    for(auto it = objects.begin(); it != objects.end(); it++)
    {
        if(it->first.baseId == id)
        {
            delete it->second;
            objects.erase(it);
            return;
        }
    }
}

GPOREGISTRY_TEMPLATE
void GPORegistry<IdT, ObjT>::removeByNumericId(const size_t id)
{
    for(auto it = objects.begin(); it != objects.end(); it++)
    {
        if(it->first.numericId == id)
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
        if(it->first.baseId == id)
        {
            objects.erase(it);
            return;
        }
    }
}
template<class IdT>
void GPORegistry<IdT, void>::removeByNumericId(const size_t id)
{
    for(auto it = objects.begin(); it != objects.end(); it++)
    {
        if(it->first.numericId == id)
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
