#ifndef GPOREGISTRY_HPP
#define GPOREGISTRY_HPP

#include <vector>

#define GPOREGISTRY_TEMPLATE template<class IdT, class ObjT>

#define REG_ERROR_UNKNOWN -1
#define REG_ERROR_EXISTS -2

// Gameplay Object Registry.
// Note that objects MUST be created dynamically
// because there are destroyed when game is closed.
//
// Requirements for IdT:
//
// * must have operator<(IdT, IdT)
// * must have IdT::IdT() <default constructor>
// * must have [ non-explicit IdT::IdT(int) <constructor from int> and IdT operator+(IdT&, IdT&) ]
//          or IdT operator+(IdT&, int)
// * must have copy constructor or assigment operator
// * must have operator==(const IdT&, const IdT&)
// The best for it will be number primitive type e.g. int.
//
// The ObjT can be anything.
GPOREGISTRY_TEMPLATE
class GPORegistry
{
public:

    typedef std::vector<std::pair<IdT, ObjT*>> ArrayType;

    GPORegistry();

    virtual ~GPORegistry();

    // Deallocates and removes all elements from registry.
    virtual void clear();

    // Adds a new object to registry. It must be created dynamically.
    // Returns negative value if object exists (or another error occured), 0
    // otherwise.
    virtual int add(IdT id, ObjT* obj);

    // Adds an new object, generating a new ID for it. It must be created
    // dynamically. Returns an ID of object or 0 if error.
    virtual IdT add(ObjT* obj);

    // Returns an object that has specified %id. Returns NULL if
    // the object doesn't exist or another error occured.
    virtual ObjT* findById(const IdT& id) const;

    // Returns an ID of %obj. Returns IdT() if object doesn't exist
    // or another error occured.
    virtual IdT getIdOf(ObjT* obj) const;

    // Removes an object with specified %id.
    virtual void remove(const IdT& id);

    // Removes (but not deallocates) object with the same pointer
    // that specified.
    virtual void remove(ObjT* obj);

    // Returns count of objects registered in this registry.
    virtual int count() const;

    // Returns a reference to internal array. Can be useful for
    // iterating on elements.
    virtual const ArrayType& arr() const;

private:

    ArrayType objects;
    IdT greatestId;
};

// Specialization for void - do not deallocate!!
template<class IdT>
class GPORegistry<IdT, void>
{
public:

    typedef std::vector<std::pair<IdT, void*>> ArrayType;

    // Deallocates and removes all elements from registry.
    void clear();

    // Removes an object with specified %id.
    void remove(const IdT& id);

    // Removes (but not deallocates) object with the same pointer
    // that specified.
    void remove(void* obj);

private:

    ArrayType objects;
    IdT greatestId;
};

// Functions
#include "GPORegistryImplDefault.inl"

#endif // GPOREGISTRY_HPP

