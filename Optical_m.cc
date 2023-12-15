//
// Generated file, do not edit! Created by nedtool 5.4 from Optical.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include "Optical_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i=0; i<n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp


// forward
template<typename T, typename A>
std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec);

// Template rule which fires if a struct or class doesn't have operator<<
template<typename T>
inline std::ostream& operator<<(std::ostream& out,const T&) {return out;}

// operator<< for std::vector<T>
template<typename T, typename A>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T,A>& vec)
{
    out.put('{');
    for(typename std::vector<T,A>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        if (it != vec.begin()) {
            out.put(','); out.put(' ');
        }
        out << *it;
    }
    out.put('}');
    
    char buf[32];
    sprintf(buf, " (size=%u)", (unsigned int)vec.size());
    out.write(buf, strlen(buf));
    return out;
}

Register_Class(Optical)

Optical::Optical(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->PreviousToR = 0;
    this->NextToR = 0;
    this->TxIndex = 0;
    this->GroupToRIndex = 0;
    this->optical_num = 0;
    this->index = 0;
    this->app_pkt_num = 0;
}

Optical::Optical(const Optical& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

Optical::~Optical()
{
}

Optical& Optical::operator=(const Optical& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void Optical::copy(const Optical& other)
{
    this->pkt1 = other.pkt1;
    this->pkt2 = other.pkt2;
    this->pkt3 = other.pkt3;
    this->pkt4 = other.pkt4;
    this->pkt5 = other.pkt5;
    this->pkt6 = other.pkt6;
    this->pkt7 = other.pkt7;
    this->pkt8 = other.pkt8;
    this->pkt9 = other.pkt9;
    this->pkt10 = other.pkt10;
    this->pkt11 = other.pkt11;
    this->pkt12 = other.pkt12;
    this->pkt13 = other.pkt13;
    this->pkt14 = other.pkt14;
    this->pkt15 = other.pkt15;
    this->pkt16 = other.pkt16;
    this->pkt17 = other.pkt17;
    this->pkt18 = other.pkt18;
    this->pkt19 = other.pkt19;
    this->pkt20 = other.pkt20;
    this->pkt21 = other.pkt21;
    this->pkt22 = other.pkt22;
    this->pkt23 = other.pkt23;
    this->pkt24 = other.pkt24;
    this->pkt25 = other.pkt25;
    this->PreviousToR = other.PreviousToR;
    this->NextToR = other.NextToR;
    this->TxIndex = other.TxIndex;
    this->GroupToRIndex = other.GroupToRIndex;
    this->optical_num = other.optical_num;
    this->index = other.index;
    this->Optical_pkts = other.Optical_pkts;
    this->app_pkt_num = other.app_pkt_num;
}

void Optical::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->pkt1);
    doParsimPacking(b,this->pkt2);
    doParsimPacking(b,this->pkt3);
    doParsimPacking(b,this->pkt4);
    doParsimPacking(b,this->pkt5);
    doParsimPacking(b,this->pkt6);
    doParsimPacking(b,this->pkt7);
    doParsimPacking(b,this->pkt8);
    doParsimPacking(b,this->pkt9);
    doParsimPacking(b,this->pkt10);
    doParsimPacking(b,this->pkt11);
    doParsimPacking(b,this->pkt12);
    doParsimPacking(b,this->pkt13);
    doParsimPacking(b,this->pkt14);
    doParsimPacking(b,this->pkt15);
    doParsimPacking(b,this->pkt16);
    doParsimPacking(b,this->pkt17);
    doParsimPacking(b,this->pkt18);
    doParsimPacking(b,this->pkt19);
    doParsimPacking(b,this->pkt20);
    doParsimPacking(b,this->pkt21);
    doParsimPacking(b,this->pkt22);
    doParsimPacking(b,this->pkt23);
    doParsimPacking(b,this->pkt24);
    doParsimPacking(b,this->pkt25);
    doParsimPacking(b,this->PreviousToR);
    doParsimPacking(b,this->NextToR);
    doParsimPacking(b,this->TxIndex);
    doParsimPacking(b,this->GroupToRIndex);
    doParsimPacking(b,this->optical_num);
    doParsimPacking(b,this->index);
    doParsimPacking(b,this->Optical_pkts);
    doParsimPacking(b,this->app_pkt_num);
}

void Optical::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->pkt1);
    doParsimUnpacking(b,this->pkt2);
    doParsimUnpacking(b,this->pkt3);
    doParsimUnpacking(b,this->pkt4);
    doParsimUnpacking(b,this->pkt5);
    doParsimUnpacking(b,this->pkt6);
    doParsimUnpacking(b,this->pkt7);
    doParsimUnpacking(b,this->pkt8);
    doParsimUnpacking(b,this->pkt9);
    doParsimUnpacking(b,this->pkt10);
    doParsimUnpacking(b,this->pkt11);
    doParsimUnpacking(b,this->pkt12);
    doParsimUnpacking(b,this->pkt13);
    doParsimUnpacking(b,this->pkt14);
    doParsimUnpacking(b,this->pkt15);
    doParsimUnpacking(b,this->pkt16);
    doParsimUnpacking(b,this->pkt17);
    doParsimUnpacking(b,this->pkt18);
    doParsimUnpacking(b,this->pkt19);
    doParsimUnpacking(b,this->pkt20);
    doParsimUnpacking(b,this->pkt21);
    doParsimUnpacking(b,this->pkt22);
    doParsimUnpacking(b,this->pkt23);
    doParsimUnpacking(b,this->pkt24);
    doParsimUnpacking(b,this->pkt25);
    doParsimUnpacking(b,this->PreviousToR);
    doParsimUnpacking(b,this->NextToR);
    doParsimUnpacking(b,this->TxIndex);
    doParsimUnpacking(b,this->GroupToRIndex);
    doParsimUnpacking(b,this->optical_num);
    doParsimUnpacking(b,this->index);
    doParsimUnpacking(b,this->Optical_pkts);
    doParsimUnpacking(b,this->app_pkt_num);
}

pkt_Ptr& Optical::getPkt1()
{
    return this->pkt1;
}

void Optical::setPkt1(const pkt_Ptr& pkt1)
{
    this->pkt1 = pkt1;
}

pkt_Ptr& Optical::getPkt2()
{
    return this->pkt2;
}

void Optical::setPkt2(const pkt_Ptr& pkt2)
{
    this->pkt2 = pkt2;
}

pkt_Ptr& Optical::getPkt3()
{
    return this->pkt3;
}

void Optical::setPkt3(const pkt_Ptr& pkt3)
{
    this->pkt3 = pkt3;
}

pkt_Ptr& Optical::getPkt4()
{
    return this->pkt4;
}

void Optical::setPkt4(const pkt_Ptr& pkt4)
{
    this->pkt4 = pkt4;
}

pkt_Ptr& Optical::getPkt5()
{
    return this->pkt5;
}

void Optical::setPkt5(const pkt_Ptr& pkt5)
{
    this->pkt5 = pkt5;
}

pkt_Ptr& Optical::getPkt6()
{
    return this->pkt6;
}

void Optical::setPkt6(const pkt_Ptr& pkt6)
{
    this->pkt6 = pkt6;
}

pkt_Ptr& Optical::getPkt7()
{
    return this->pkt7;
}

void Optical::setPkt7(const pkt_Ptr& pkt7)
{
    this->pkt7 = pkt7;
}

pkt_Ptr& Optical::getPkt8()
{
    return this->pkt8;
}

void Optical::setPkt8(const pkt_Ptr& pkt8)
{
    this->pkt8 = pkt8;
}

pkt_Ptr& Optical::getPkt9()
{
    return this->pkt9;
}

void Optical::setPkt9(const pkt_Ptr& pkt9)
{
    this->pkt9 = pkt9;
}

pkt_Ptr& Optical::getPkt10()
{
    return this->pkt10;
}

void Optical::setPkt10(const pkt_Ptr& pkt10)
{
    this->pkt10 = pkt10;
}

pkt_Ptr& Optical::getPkt11()
{
    return this->pkt11;
}

void Optical::setPkt11(const pkt_Ptr& pkt11)
{
    this->pkt11 = pkt11;
}

pkt_Ptr& Optical::getPkt12()
{
    return this->pkt12;
}

void Optical::setPkt12(const pkt_Ptr& pkt12)
{
    this->pkt12 = pkt12;
}

pkt_Ptr& Optical::getPkt13()
{
    return this->pkt13;
}

void Optical::setPkt13(const pkt_Ptr& pkt13)
{
    this->pkt13 = pkt13;
}

pkt_Ptr& Optical::getPkt14()
{
    return this->pkt14;
}

void Optical::setPkt14(const pkt_Ptr& pkt14)
{
    this->pkt14 = pkt14;
}

pkt_Ptr& Optical::getPkt15()
{
    return this->pkt15;
}

void Optical::setPkt15(const pkt_Ptr& pkt15)
{
    this->pkt15 = pkt15;
}

pkt_Ptr& Optical::getPkt16()
{
    return this->pkt16;
}

void Optical::setPkt16(const pkt_Ptr& pkt16)
{
    this->pkt16 = pkt16;
}

pkt_Ptr& Optical::getPkt17()
{
    return this->pkt17;
}

void Optical::setPkt17(const pkt_Ptr& pkt17)
{
    this->pkt17 = pkt17;
}

pkt_Ptr& Optical::getPkt18()
{
    return this->pkt18;
}

void Optical::setPkt18(const pkt_Ptr& pkt18)
{
    this->pkt18 = pkt18;
}

pkt_Ptr& Optical::getPkt19()
{
    return this->pkt19;
}

void Optical::setPkt19(const pkt_Ptr& pkt19)
{
    this->pkt19 = pkt19;
}

pkt_Ptr& Optical::getPkt20()
{
    return this->pkt20;
}

void Optical::setPkt20(const pkt_Ptr& pkt20)
{
    this->pkt20 = pkt20;
}

pkt_Ptr& Optical::getPkt21()
{
    return this->pkt21;
}

void Optical::setPkt21(const pkt_Ptr& pkt21)
{
    this->pkt21 = pkt21;
}

pkt_Ptr& Optical::getPkt22()
{
    return this->pkt22;
}

void Optical::setPkt22(const pkt_Ptr& pkt22)
{
    this->pkt22 = pkt22;
}

pkt_Ptr& Optical::getPkt23()
{
    return this->pkt23;
}

void Optical::setPkt23(const pkt_Ptr& pkt23)
{
    this->pkt23 = pkt23;
}

pkt_Ptr& Optical::getPkt24()
{
    return this->pkt24;
}

void Optical::setPkt24(const pkt_Ptr& pkt24)
{
    this->pkt24 = pkt24;
}

pkt_Ptr& Optical::getPkt25()
{
    return this->pkt25;
}

void Optical::setPkt25(const pkt_Ptr& pkt25)
{
    this->pkt25 = pkt25;
}

int Optical::getPreviousToR() const
{
    return this->PreviousToR;
}

void Optical::setPreviousToR(int PreviousToR)
{
    this->PreviousToR = PreviousToR;
}

int Optical::getNextToR() const
{
    return this->NextToR;
}

void Optical::setNextToR(int NextToR)
{
    this->NextToR = NextToR;
}

int Optical::getTxIndex() const
{
    return this->TxIndex;
}

void Optical::setTxIndex(int TxIndex)
{
    this->TxIndex = TxIndex;
}

int Optical::getGroupToRIndex() const
{
    return this->GroupToRIndex;
}

void Optical::setGroupToRIndex(int GroupToRIndex)
{
    this->GroupToRIndex = GroupToRIndex;
}

int Optical::getOptical_num() const
{
    return this->optical_num;
}

void Optical::setOptical_num(int optical_num)
{
    this->optical_num = optical_num;
}

int Optical::getIndex() const
{
    return this->index;
}

void Optical::setIndex(int index)
{
    this->index = index;
}

PktVector& Optical::getOptical_pkts()
{
    return this->Optical_pkts;
}

void Optical::setOptical_pkts(const PktVector& Optical_pkts)
{
    this->Optical_pkts = Optical_pkts;
}

int Optical::getApp_pkt_num() const
{
    return this->app_pkt_num;
}

void Optical::setApp_pkt_num(int app_pkt_num)
{
    this->app_pkt_num = app_pkt_num;
}


void Optical::insertMessage(pkt *msg)
{
take(msg); // take ownership
//Optical_pkts.insert(msg); // store pointer
Optical_pkts.push_back(msg);
}

void Optical::removeMessage(pkt * msg)
{
Optical_pkts.remove(msg);
drop(msg); // release ownership
}

class OpticalDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    OpticalDescriptor();
    virtual ~OpticalDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyname) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyname) const override;
    virtual int getFieldArraySize(void *object, int field) const override;

    virtual const char *getFieldDynamicTypeString(void *object, int field, int i) const override;
    virtual std::string getFieldValueAsString(void *object, int field, int i) const override;
    virtual bool setFieldValueAsString(void *object, int field, int i, const char *value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual void *getFieldStructValuePointer(void *object, int field, int i) const override;
};

Register_ClassDescriptor(OpticalDescriptor)

OpticalDescriptor::OpticalDescriptor() : omnetpp::cClassDescriptor("Optical", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

OpticalDescriptor::~OpticalDescriptor()
{
    delete[] propertynames;
}

bool OpticalDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<Optical *>(obj)!=nullptr;
}

const char **OpticalDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *OpticalDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int OpticalDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 33+basedesc->getFieldCount() : 33;
}

unsigned int OpticalDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISCOMPOUND,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<33) ? fieldTypeFlags[field] : 0;
}

const char *OpticalDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "pkt1",
        "pkt2",
        "pkt3",
        "pkt4",
        "pkt5",
        "pkt6",
        "pkt7",
        "pkt8",
        "pkt9",
        "pkt10",
        "pkt11",
        "pkt12",
        "pkt13",
        "pkt14",
        "pkt15",
        "pkt16",
        "pkt17",
        "pkt18",
        "pkt19",
        "pkt20",
        "pkt21",
        "pkt22",
        "pkt23",
        "pkt24",
        "pkt25",
        "PreviousToR",
        "NextToR",
        "TxIndex",
        "GroupToRIndex",
        "optical_num",
        "index",
        "Optical_pkts",
        "app_pkt_num",
    };
    return (field>=0 && field<33) ? fieldNames[field] : nullptr;
}

int OpticalDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt1")==0) return base+0;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt2")==0) return base+1;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt3")==0) return base+2;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt4")==0) return base+3;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt5")==0) return base+4;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt6")==0) return base+5;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt7")==0) return base+6;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt8")==0) return base+7;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt9")==0) return base+8;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt10")==0) return base+9;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt11")==0) return base+10;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt12")==0) return base+11;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt13")==0) return base+12;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt14")==0) return base+13;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt15")==0) return base+14;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt16")==0) return base+15;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt17")==0) return base+16;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt18")==0) return base+17;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt19")==0) return base+18;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt20")==0) return base+19;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt21")==0) return base+20;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt22")==0) return base+21;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt23")==0) return base+22;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt24")==0) return base+23;
    if (fieldName[0]=='p' && strcmp(fieldName, "pkt25")==0) return base+24;
    if (fieldName[0]=='P' && strcmp(fieldName, "PreviousToR")==0) return base+25;
    if (fieldName[0]=='N' && strcmp(fieldName, "NextToR")==0) return base+26;
    if (fieldName[0]=='T' && strcmp(fieldName, "TxIndex")==0) return base+27;
    if (fieldName[0]=='G' && strcmp(fieldName, "GroupToRIndex")==0) return base+28;
    if (fieldName[0]=='o' && strcmp(fieldName, "optical_num")==0) return base+29;
    if (fieldName[0]=='i' && strcmp(fieldName, "index")==0) return base+30;
    if (fieldName[0]=='O' && strcmp(fieldName, "Optical_pkts")==0) return base+31;
    if (fieldName[0]=='a' && strcmp(fieldName, "app_pkt_num")==0) return base+32;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *OpticalDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "pkt_Ptr",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "PktVector",
        "int",
    };
    return (field>=0 && field<33) ? fieldTypeStrings[field] : nullptr;
}

const char **OpticalDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldPropertyNames(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *OpticalDescriptor::getFieldProperty(int field, const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldProperty(field, propertyname);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int OpticalDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    Optical *pp = (Optical *)object; (void)pp;
    switch (field) {
        default: return 0;
    }
}

const char *OpticalDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Optical *pp = (Optical *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string OpticalDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    Optical *pp = (Optical *)object; (void)pp;
    switch (field) {
        case 0: {std::stringstream out; out << pp->getPkt1(); return out.str();}
        case 1: {std::stringstream out; out << pp->getPkt2(); return out.str();}
        case 2: {std::stringstream out; out << pp->getPkt3(); return out.str();}
        case 3: {std::stringstream out; out << pp->getPkt4(); return out.str();}
        case 4: {std::stringstream out; out << pp->getPkt5(); return out.str();}
        case 5: {std::stringstream out; out << pp->getPkt6(); return out.str();}
        case 6: {std::stringstream out; out << pp->getPkt7(); return out.str();}
        case 7: {std::stringstream out; out << pp->getPkt8(); return out.str();}
        case 8: {std::stringstream out; out << pp->getPkt9(); return out.str();}
        case 9: {std::stringstream out; out << pp->getPkt10(); return out.str();}
        case 10: {std::stringstream out; out << pp->getPkt11(); return out.str();}
        case 11: {std::stringstream out; out << pp->getPkt12(); return out.str();}
        case 12: {std::stringstream out; out << pp->getPkt13(); return out.str();}
        case 13: {std::stringstream out; out << pp->getPkt14(); return out.str();}
        case 14: {std::stringstream out; out << pp->getPkt15(); return out.str();}
        case 15: {std::stringstream out; out << pp->getPkt16(); return out.str();}
        case 16: {std::stringstream out; out << pp->getPkt17(); return out.str();}
        case 17: {std::stringstream out; out << pp->getPkt18(); return out.str();}
        case 18: {std::stringstream out; out << pp->getPkt19(); return out.str();}
        case 19: {std::stringstream out; out << pp->getPkt20(); return out.str();}
        case 20: {std::stringstream out; out << pp->getPkt21(); return out.str();}
        case 21: {std::stringstream out; out << pp->getPkt22(); return out.str();}
        case 22: {std::stringstream out; out << pp->getPkt23(); return out.str();}
        case 23: {std::stringstream out; out << pp->getPkt24(); return out.str();}
        case 24: {std::stringstream out; out << pp->getPkt25(); return out.str();}
        case 25: return long2string(pp->getPreviousToR());
        case 26: return long2string(pp->getNextToR());
        case 27: return long2string(pp->getTxIndex());
        case 28: return long2string(pp->getGroupToRIndex());
        case 29: return long2string(pp->getOptical_num());
        case 30: return long2string(pp->getIndex());
        case 31: {std::stringstream out; out << pp->getOptical_pkts(); return out.str();}
        case 32: return long2string(pp->getApp_pkt_num());
        default: return "";
    }
}

bool OpticalDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    Optical *pp = (Optical *)object; (void)pp;
    switch (field) {
        case 25: pp->setPreviousToR(string2long(value)); return true;
        case 26: pp->setNextToR(string2long(value)); return true;
        case 27: pp->setTxIndex(string2long(value)); return true;
        case 28: pp->setGroupToRIndex(string2long(value)); return true;
        case 29: pp->setOptical_num(string2long(value)); return true;
        case 30: pp->setIndex(string2long(value)); return true;
        case 32: pp->setApp_pkt_num(string2long(value)); return true;
        default: return false;
    }
}

const char *OpticalDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        case 0: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 1: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 2: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 3: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 4: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 5: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 6: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 7: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 8: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 9: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 10: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 11: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 12: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 13: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 14: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 15: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 16: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 17: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 18: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 19: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 20: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 21: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 22: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 23: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 24: return omnetpp::opp_typename(typeid(pkt_Ptr));
        case 31: return omnetpp::opp_typename(typeid(PktVector));
        default: return nullptr;
    };
}

void *OpticalDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    Optical *pp = (Optical *)object; (void)pp;
    switch (field) {
        case 0: return (void *)(&pp->getPkt1()); break;
        case 1: return (void *)(&pp->getPkt2()); break;
        case 2: return (void *)(&pp->getPkt3()); break;
        case 3: return (void *)(&pp->getPkt4()); break;
        case 4: return (void *)(&pp->getPkt5()); break;
        case 5: return (void *)(&pp->getPkt6()); break;
        case 6: return (void *)(&pp->getPkt7()); break;
        case 7: return (void *)(&pp->getPkt8()); break;
        case 8: return (void *)(&pp->getPkt9()); break;
        case 9: return (void *)(&pp->getPkt10()); break;
        case 10: return (void *)(&pp->getPkt11()); break;
        case 11: return (void *)(&pp->getPkt12()); break;
        case 12: return (void *)(&pp->getPkt13()); break;
        case 13: return (void *)(&pp->getPkt14()); break;
        case 14: return (void *)(&pp->getPkt15()); break;
        case 15: return (void *)(&pp->getPkt16()); break;
        case 16: return (void *)(&pp->getPkt17()); break;
        case 17: return (void *)(&pp->getPkt18()); break;
        case 18: return (void *)(&pp->getPkt19()); break;
        case 19: return (void *)(&pp->getPkt20()); break;
        case 20: return (void *)(&pp->getPkt21()); break;
        case 21: return (void *)(&pp->getPkt22()); break;
        case 22: return (void *)(&pp->getPkt23()); break;
        case 23: return (void *)(&pp->getPkt24()); break;
        case 24: return (void *)(&pp->getPkt25()); break;
        case 31: return (void *)(&pp->getOptical_pkts()); break;
        default: return nullptr;
    }
}


