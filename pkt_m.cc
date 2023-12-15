//
// Generated file, do not edit! Created by nedtool 5.4 from pkt.msg.
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
#include "pkt_m.h"

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

Register_Class(pkt)

pkt::pkt(const char *name, short kind) : ::omnetpp::cPacket(name,kind)
{
    this->src_Server = 0;
    this->dst_Server = 0;
    this->src_ToR = 0;
    this->dst_ToR = 0;
    this->index = 0;
    this->TotalHopCount = 0;
    this->HopCountNow = 0;
    for (unsigned int i=0; i<11; i++)
        this->Hop[i] = 0;
    this->PacketSize = 0;
    this->no = 0;
    this->m = 0;
    this->retransmission = 0;
    this->path = 0;
    this->TxIndex = 0;
    this->GroupToRIndex = 0;
    this->End2EndStartTime = 0;
    this->PacketNetworkStartTime = 0;
    this->PacketEnd2EndStartTime = 0;
    this->flag_intraToR = 0;
    this->flag_ToR2ToR = 0;
    this->optNum = 0;
    this->flag_last = 0;
}

pkt::pkt(const pkt& other) : ::omnetpp::cPacket(other)
{
    copy(other);
}

pkt::~pkt()
{
}

pkt& pkt::operator=(const pkt& other)
{
    if (this==&other) return *this;
    ::omnetpp::cPacket::operator=(other);
    copy(other);
    return *this;
}

void pkt::copy(const pkt& other)
{
    this->src_Server = other.src_Server;
    this->dst_Server = other.dst_Server;
    this->src_ToR = other.src_ToR;
    this->dst_ToR = other.dst_ToR;
    this->index = other.index;
    this->TotalHopCount = other.TotalHopCount;
    this->HopCountNow = other.HopCountNow;
    for (unsigned int i=0; i<11; i++)
        this->Hop[i] = other.Hop[i];
    this->PacketSize = other.PacketSize;
    this->no = other.no;
    this->m = other.m;
    this->retransmission = other.retransmission;
    this->path = other.path;
    this->TxIndex = other.TxIndex;
    this->GroupToRIndex = other.GroupToRIndex;
    this->End2EndStartTime = other.End2EndStartTime;
    this->PacketNetworkStartTime = other.PacketNetworkStartTime;
    this->PacketEnd2EndStartTime = other.PacketEnd2EndStartTime;
    this->flag_intraToR = other.flag_intraToR;
    this->flag_ToR2ToR = other.flag_ToR2ToR;
    this->optNum = other.optNum;
    this->flag_last = other.flag_last;
}

void pkt::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cPacket::parsimPack(b);
    doParsimPacking(b,this->src_Server);
    doParsimPacking(b,this->dst_Server);
    doParsimPacking(b,this->src_ToR);
    doParsimPacking(b,this->dst_ToR);
    doParsimPacking(b,this->index);
    doParsimPacking(b,this->TotalHopCount);
    doParsimPacking(b,this->HopCountNow);
    doParsimArrayPacking(b,this->Hop,11);
    doParsimPacking(b,this->PacketSize);
    doParsimPacking(b,this->no);
    doParsimPacking(b,this->m);
    doParsimPacking(b,this->retransmission);
    doParsimPacking(b,this->path);
    doParsimPacking(b,this->TxIndex);
    doParsimPacking(b,this->GroupToRIndex);
    doParsimPacking(b,this->End2EndStartTime);
    doParsimPacking(b,this->PacketNetworkStartTime);
    doParsimPacking(b,this->PacketEnd2EndStartTime);
    doParsimPacking(b,this->flag_intraToR);
    doParsimPacking(b,this->flag_ToR2ToR);
    doParsimPacking(b,this->optNum);
    doParsimPacking(b,this->flag_last);
}

void pkt::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cPacket::parsimUnpack(b);
    doParsimUnpacking(b,this->src_Server);
    doParsimUnpacking(b,this->dst_Server);
    doParsimUnpacking(b,this->src_ToR);
    doParsimUnpacking(b,this->dst_ToR);
    doParsimUnpacking(b,this->index);
    doParsimUnpacking(b,this->TotalHopCount);
    doParsimUnpacking(b,this->HopCountNow);
    doParsimArrayUnpacking(b,this->Hop,11);
    doParsimUnpacking(b,this->PacketSize);
    doParsimUnpacking(b,this->no);
    doParsimUnpacking(b,this->m);
    doParsimUnpacking(b,this->retransmission);
    doParsimUnpacking(b,this->path);
    doParsimUnpacking(b,this->TxIndex);
    doParsimUnpacking(b,this->GroupToRIndex);
    doParsimUnpacking(b,this->End2EndStartTime);
    doParsimUnpacking(b,this->PacketNetworkStartTime);
    doParsimUnpacking(b,this->PacketEnd2EndStartTime);
    doParsimUnpacking(b,this->flag_intraToR);
    doParsimUnpacking(b,this->flag_ToR2ToR);
    doParsimUnpacking(b,this->optNum);
    doParsimUnpacking(b,this->flag_last);
}

int pkt::getSrc_Server() const
{
    return this->src_Server;
}

void pkt::setSrc_Server(int src_Server)
{
    this->src_Server = src_Server;
}

int pkt::getDst_Server() const
{
    return this->dst_Server;
}

void pkt::setDst_Server(int dst_Server)
{
    this->dst_Server = dst_Server;
}

int pkt::getSrc_ToR() const
{
    return this->src_ToR;
}

void pkt::setSrc_ToR(int src_ToR)
{
    this->src_ToR = src_ToR;
}

int pkt::getDst_ToR() const
{
    return this->dst_ToR;
}

void pkt::setDst_ToR(int dst_ToR)
{
    this->dst_ToR = dst_ToR;
}

int pkt::getIndex() const
{
    return this->index;
}

void pkt::setIndex(int index)
{
    this->index = index;
}

int pkt::getTotalHopCount() const
{
    return this->TotalHopCount;
}

void pkt::setTotalHopCount(int TotalHopCount)
{
    this->TotalHopCount = TotalHopCount;
}

int pkt::getHopCountNow() const
{
    return this->HopCountNow;
}

void pkt::setHopCountNow(int HopCountNow)
{
    this->HopCountNow = HopCountNow;
}

unsigned int pkt::getHopArraySize() const
{
    return 11;
}

int pkt::getHop(unsigned int k) const
{
    if (k>=11) throw omnetpp::cRuntimeError("Array of size 11 indexed by %lu", (unsigned long)k);
    return this->Hop[k];
}

void pkt::setHop(unsigned int k, int Hop)
{
    if (k>=11) throw omnetpp::cRuntimeError("Array of size 11 indexed by %lu", (unsigned long)k);
    this->Hop[k] = Hop;
}

int pkt::getPacketSize() const
{
    return this->PacketSize;
}

void pkt::setPacketSize(int PacketSize)
{
    this->PacketSize = PacketSize;
}

int pkt::getNo() const
{
    return this->no;
}

void pkt::setNo(int no)
{
    this->no = no;
}

int pkt::getM() const
{
    return this->m;
}

void pkt::setM(int m)
{
    this->m = m;
}

int pkt::getRetransmission() const
{
    return this->retransmission;
}

void pkt::setRetransmission(int retransmission)
{
    this->retransmission = retransmission;
}

int pkt::getPath() const
{
    return this->path;
}

void pkt::setPath(int path)
{
    this->path = path;
}

int pkt::getTxIndex() const
{
    return this->TxIndex;
}

void pkt::setTxIndex(int TxIndex)
{
    this->TxIndex = TxIndex;
}

int pkt::getGroupToRIndex() const
{
    return this->GroupToRIndex;
}

void pkt::setGroupToRIndex(int GroupToRIndex)
{
    this->GroupToRIndex = GroupToRIndex;
}

double pkt::getEnd2EndStartTime() const
{
    return this->End2EndStartTime;
}

void pkt::setEnd2EndStartTime(double End2EndStartTime)
{
    this->End2EndStartTime = End2EndStartTime;
}

::omnetpp::simtime_t pkt::getPacketNetworkStartTime() const
{
    return this->PacketNetworkStartTime;
}

void pkt::setPacketNetworkStartTime(::omnetpp::simtime_t PacketNetworkStartTime)
{
    this->PacketNetworkStartTime = PacketNetworkStartTime;
}

::omnetpp::simtime_t pkt::getPacketEnd2EndStartTime() const
{
    return this->PacketEnd2EndStartTime;
}

void pkt::setPacketEnd2EndStartTime(::omnetpp::simtime_t PacketEnd2EndStartTime)
{
    this->PacketEnd2EndStartTime = PacketEnd2EndStartTime;
}

int pkt::getFlag_intraToR() const
{
    return this->flag_intraToR;
}

void pkt::setFlag_intraToR(int flag_intraToR)
{
    this->flag_intraToR = flag_intraToR;
}

int pkt::getFlag_ToR2ToR() const
{
    return this->flag_ToR2ToR;
}

void pkt::setFlag_ToR2ToR(int flag_ToR2ToR)
{
    this->flag_ToR2ToR = flag_ToR2ToR;
}

int pkt::getOptNum() const
{
    return this->optNum;
}

void pkt::setOptNum(int optNum)
{
    this->optNum = optNum;
}

int pkt::getFlag_last() const
{
    return this->flag_last;
}

void pkt::setFlag_last(int flag_last)
{
    this->flag_last = flag_last;
}

class pktDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertynames;
  public:
    pktDescriptor();
    virtual ~pktDescriptor();

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

Register_ClassDescriptor(pktDescriptor)

pktDescriptor::pktDescriptor() : omnetpp::cClassDescriptor("pkt", "omnetpp::cPacket")
{
    propertynames = nullptr;
}

pktDescriptor::~pktDescriptor()
{
    delete[] propertynames;
}

bool pktDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<pkt *>(obj)!=nullptr;
}

const char **pktDescriptor::getPropertyNames() const
{
    if (!propertynames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
        const char **basenames = basedesc ? basedesc->getPropertyNames() : nullptr;
        propertynames = mergeLists(basenames, names);
    }
    return propertynames;
}

const char *pktDescriptor::getProperty(const char *propertyname) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? basedesc->getProperty(propertyname) : nullptr;
}

int pktDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    return basedesc ? 22+basedesc->getFieldCount() : 22;
}

unsigned int pktDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeFlags(field);
        field -= basedesc->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISARRAY | FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
        FD_ISEDITABLE,
    };
    return (field>=0 && field<22) ? fieldTypeFlags[field] : 0;
}

const char *pktDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldName(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldNames[] = {
        "src_Server",
        "dst_Server",
        "src_ToR",
        "dst_ToR",
        "index",
        "TotalHopCount",
        "HopCountNow",
        "Hop",
        "PacketSize",
        "no",
        "m",
        "retransmission",
        "path",
        "TxIndex",
        "GroupToRIndex",
        "End2EndStartTime",
        "PacketNetworkStartTime",
        "PacketEnd2EndStartTime",
        "flag_intraToR",
        "flag_ToR2ToR",
        "optNum",
        "flag_last",
    };
    return (field>=0 && field<22) ? fieldNames[field] : nullptr;
}

int pktDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    int base = basedesc ? basedesc->getFieldCount() : 0;
    if (fieldName[0]=='s' && strcmp(fieldName, "src_Server")==0) return base+0;
    if (fieldName[0]=='d' && strcmp(fieldName, "dst_Server")==0) return base+1;
    if (fieldName[0]=='s' && strcmp(fieldName, "src_ToR")==0) return base+2;
    if (fieldName[0]=='d' && strcmp(fieldName, "dst_ToR")==0) return base+3;
    if (fieldName[0]=='i' && strcmp(fieldName, "index")==0) return base+4;
    if (fieldName[0]=='T' && strcmp(fieldName, "TotalHopCount")==0) return base+5;
    if (fieldName[0]=='H' && strcmp(fieldName, "HopCountNow")==0) return base+6;
    if (fieldName[0]=='H' && strcmp(fieldName, "Hop")==0) return base+7;
    if (fieldName[0]=='P' && strcmp(fieldName, "PacketSize")==0) return base+8;
    if (fieldName[0]=='n' && strcmp(fieldName, "no")==0) return base+9;
    if (fieldName[0]=='m' && strcmp(fieldName, "m")==0) return base+10;
    if (fieldName[0]=='r' && strcmp(fieldName, "retransmission")==0) return base+11;
    if (fieldName[0]=='p' && strcmp(fieldName, "path")==0) return base+12;
    if (fieldName[0]=='T' && strcmp(fieldName, "TxIndex")==0) return base+13;
    if (fieldName[0]=='G' && strcmp(fieldName, "GroupToRIndex")==0) return base+14;
    if (fieldName[0]=='E' && strcmp(fieldName, "End2EndStartTime")==0) return base+15;
    if (fieldName[0]=='P' && strcmp(fieldName, "PacketNetworkStartTime")==0) return base+16;
    if (fieldName[0]=='P' && strcmp(fieldName, "PacketEnd2EndStartTime")==0) return base+17;
    if (fieldName[0]=='f' && strcmp(fieldName, "flag_intraToR")==0) return base+18;
    if (fieldName[0]=='f' && strcmp(fieldName, "flag_ToR2ToR")==0) return base+19;
    if (fieldName[0]=='o' && strcmp(fieldName, "optNum")==0) return base+20;
    if (fieldName[0]=='f' && strcmp(fieldName, "flag_last")==0) return base+21;
    return basedesc ? basedesc->findField(fieldName) : -1;
}

const char *pktDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldTypeString(field);
        field -= basedesc->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "int",
        "double",
        "simtime_t",
        "simtime_t",
        "int",
        "int",
        "int",
        "int",
    };
    return (field>=0 && field<22) ? fieldTypeStrings[field] : nullptr;
}

const char **pktDescriptor::getFieldPropertyNames(int field) const
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

const char *pktDescriptor::getFieldProperty(int field, const char *propertyname) const
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

int pktDescriptor::getFieldArraySize(void *object, int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldArraySize(object, field);
        field -= basedesc->getFieldCount();
    }
    pkt *pp = (pkt *)object; (void)pp;
    switch (field) {
        case 7: return 11;
        default: return 0;
    }
}

const char *pktDescriptor::getFieldDynamicTypeString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldDynamicTypeString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    pkt *pp = (pkt *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string pktDescriptor::getFieldValueAsString(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldValueAsString(object,field,i);
        field -= basedesc->getFieldCount();
    }
    pkt *pp = (pkt *)object; (void)pp;
    switch (field) {
        case 0: return long2string(pp->getSrc_Server());
        case 1: return long2string(pp->getDst_Server());
        case 2: return long2string(pp->getSrc_ToR());
        case 3: return long2string(pp->getDst_ToR());
        case 4: return long2string(pp->getIndex());
        case 5: return long2string(pp->getTotalHopCount());
        case 6: return long2string(pp->getHopCountNow());
        case 7: return long2string(pp->getHop(i));
        case 8: return long2string(pp->getPacketSize());
        case 9: return long2string(pp->getNo());
        case 10: return long2string(pp->getM());
        case 11: return long2string(pp->getRetransmission());
        case 12: return long2string(pp->getPath());
        case 13: return long2string(pp->getTxIndex());
        case 14: return long2string(pp->getGroupToRIndex());
        case 15: return double2string(pp->getEnd2EndStartTime());
        case 16: return simtime2string(pp->getPacketNetworkStartTime());
        case 17: return simtime2string(pp->getPacketEnd2EndStartTime());
        case 18: return long2string(pp->getFlag_intraToR());
        case 19: return long2string(pp->getFlag_ToR2ToR());
        case 20: return long2string(pp->getOptNum());
        case 21: return long2string(pp->getFlag_last());
        default: return "";
    }
}

bool pktDescriptor::setFieldValueAsString(void *object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->setFieldValueAsString(object,field,i,value);
        field -= basedesc->getFieldCount();
    }
    pkt *pp = (pkt *)object; (void)pp;
    switch (field) {
        case 0: pp->setSrc_Server(string2long(value)); return true;
        case 1: pp->setDst_Server(string2long(value)); return true;
        case 2: pp->setSrc_ToR(string2long(value)); return true;
        case 3: pp->setDst_ToR(string2long(value)); return true;
        case 4: pp->setIndex(string2long(value)); return true;
        case 5: pp->setTotalHopCount(string2long(value)); return true;
        case 6: pp->setHopCountNow(string2long(value)); return true;
        case 7: pp->setHop(i,string2long(value)); return true;
        case 8: pp->setPacketSize(string2long(value)); return true;
        case 9: pp->setNo(string2long(value)); return true;
        case 10: pp->setM(string2long(value)); return true;
        case 11: pp->setRetransmission(string2long(value)); return true;
        case 12: pp->setPath(string2long(value)); return true;
        case 13: pp->setTxIndex(string2long(value)); return true;
        case 14: pp->setGroupToRIndex(string2long(value)); return true;
        case 15: pp->setEnd2EndStartTime(string2double(value)); return true;
        case 16: pp->setPacketNetworkStartTime(string2simtime(value)); return true;
        case 17: pp->setPacketEnd2EndStartTime(string2simtime(value)); return true;
        case 18: pp->setFlag_intraToR(string2long(value)); return true;
        case 19: pp->setFlag_ToR2ToR(string2long(value)); return true;
        case 20: pp->setOptNum(string2long(value)); return true;
        case 21: pp->setFlag_last(string2long(value)); return true;
        default: return false;
    }
}

const char *pktDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructName(field);
        field -= basedesc->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

void *pktDescriptor::getFieldStructValuePointer(void *object, int field, int i) const
{
    omnetpp::cClassDescriptor *basedesc = getBaseClassDescriptor();
    if (basedesc) {
        if (field < basedesc->getFieldCount())
            return basedesc->getFieldStructValuePointer(object, field, i);
        field -= basedesc->getFieldCount();
    }
    pkt *pp = (pkt *)object; (void)pp;
    switch (field) {
        default: return nullptr;
    }
}


