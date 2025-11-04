/**
 * \file
 * \brief Файл с определениями функций-членов класса \ref binaryPlist "binaryPlist"
 *
 * Класс предназначен для преобразования списка в представлении XML <a href="https://en.wikipedia.org/wiki/Property_list">Plist</a>
 * (формат version="1.0"), принятого в <a href="https://ru.wikipedia.org/wiki/MacOS">macOS</a>,
 * в двоичное представление (формат v"0?")
 */

#include <iostream>

#include <QFile>
#include <QBuffer>
#include <QXmlStreamReader>
#include <QDebug>
#include "build_icns.h"
#include "bplist.h"

/**
 * \file
 * Функции, являющиеся методами класса \ref binaryPlist "binaryPlist":
 * <BR>
 */

/**
 * \file
 * * \copybrief binaryPlist::binaryPlist()
 */
binaryPlist::binaryPlist() {

    /**
     * Алгоритм:
     *
     * Сформировать заголовок:
     *
     * * Записать "магическое" значение "bplist";
     */
    addDataString("bplist");

    /**
     * * Записать версию формата "00".
     */
    addDataString("00");

    m_objectRefSize = 0;
    m_offsetIntSize = 0;

}

/**
 * \file
 * * \copybrief binaryPlist::parser(const QString&)
 */
void binaryPlist::parser(const QString &plist) {

    QXmlStreamReader xml{QAnyStringView(plist)};
    pNode* pnode = parse_xml(xml);
    if (pnode == nullptr) {
        qCritical().noquote().nospace() << "Global error parsing XML";
        return;
    }

//    std::cout << pnode->toString().toStdString().c_str();

    for (auto i = 0; i < m_binary_list.size(); ++i) {
        switch(m_binary_list[i]->getTag()) {
            default:
                break;
            case tag_dict:
                this->addDictonaryObject(m_binary_list[i]->getRefList());
//                qDebug() << Qt::hex << "[" << i << "] " << m_binary_list[i]->tagToString() << m_binary_list[i]->getRefList();
                break;
            case tag_array:
                this->addArrayObject(m_binary_list[i]->getRefList());
//                qDebug() << Qt::hex << "[" << i << "] " << m_binary_list[i]->tagToString() << m_binary_list[i]->getRefList();
                break;
//        tag_bool,
//        tag_fill,
            case tag_int:
                this->addIntegerObject(m_binary_list[i]->getIntValue());
//                qDebug() << Qt::hex << "[" << i << "] " << m_binary_list[i]->tagToString() << m_binary_list[i]->getIntValue();
                break;
//        tag_real,
//        tag_date,
//        tag_data,
           case tag_string:
           case tag_key:
                this->addStringObject(m_binary_list[i]->getStringValue());
//                qDebug() << Qt::hex << "[" << i << "] " << m_binary_list[i]->tagToString() << m_binary_list[i]->getStringValue();
               break;
           case tag_uid:
                this->addUidObject( { static_cast<quint8>(m_binary_list[i]->getIntValue()) } );
//                qDebug() << Qt::hex << "[" << i << "] " << m_binary_list[i]->tagToString() << m_binary_list[i]->getIntValue();
               break;
        }
    }
    delete pnode;
    this->finish(0x00);
}

/**
 * \file
 * * \copybrief binaryPlist::size() const
 */
qsizetype binaryPlist::size() const {
    qsizetype size = 0;
    for (qsizetype i = 0; i < m_rawTable.size(); ++i) {
        size += element_size(m_rawTable[i]);
    }
    return size;
}

/**
 * \file
 * * \copybrief binaryPlist::output(QDataStream&) const
 */
void binaryPlist::output(QDataStream &out) const {
    for (qsizetype i = 0; i < m_rawTable.size(); ++i) {

        const qsizetype dataSize = element_size(m_rawTable[i]);
        const quint64 dataValue = m_rawTable[i].first;

        if (dataSize >= 8) {
            out << static_cast<quint8>(dataValue >> 56);
            out << static_cast<quint8>(dataValue >> 48);
            out << static_cast<quint8>(dataValue >> 40);
            out << static_cast<quint8>(dataValue >> 32);
        } 
        if (dataSize >= 4) {
            out << static_cast<quint8>(dataValue >> 24);
            out << static_cast<quint8>(dataValue >> 16);
        }
        if (dataSize >= 2) {
            out << static_cast<quint8>(dataValue >>  8);
        }
        out << static_cast<quint8>(dataValue >>  0);
    }
}

/**
 * \file
 * * \copybrief binaryPlist::element_size(const QPair<qsizetype, enum binaryPlist::TYPE>&) const
 */
qsizetype binaryPlist::element_size(const QPair<qsizetype, enum TYPE> &element) const {
    switch(element.second) {
        case CNTRL:
        case DATA8:
            return 1;
        case DATA64:
            return 8;
        case OBJOFF:
            return m_offsetIntSize;
        case OBJREF:
            return m_objectRefSize;
    }
    return 0;
}

/**
 * \file
 * * \copybrief binaryPlist::addIntegerObject(quint64, bool)
 */
void binaryPlist::addIntegerObject(quint64 val, bool isObject) {

    qsizetype start = m_rawTable.size();
    qsizetype byte_size;
    if (val <= 0xFF) {
        byte_size = 1;
        m_rawTable += { 0x10, CNTRL };
    } else if (val <= 0xFFFF) {
        byte_size = 2;
        m_rawTable += { 0x11, CNTRL };
    } else if (val <= 0xFFFFFFFF) {
        byte_size = 4;
        m_rawTable += { 0x12, CNTRL };
    }
    else {
        byte_size = 8;
        m_rawTable += { 0x13, CNTRL };
    }
    for (qsizetype i = 0; i < byte_size; ++i) {
        m_rawTable += { (val >> (8 * (byte_size - i - 1))) & 0xFF, DATA8 };
    }
    qsizetype end = m_rawTable.size();

    if (isObject) {
        m_objectIndexTable += { start, end };
    }
}

/**
 * \file
 * * \copybrief binaryPlist::addDataString(const QString&)
 */
void binaryPlist::addDataString(const QString& s) {
    for (qsizetype i = 0; i < s.size(); ++i) {
        m_rawTable += { s[i].toLatin1(), DATA8 };
    }
}

/**
 * \file
 * * \copybrief binaryPlist::objLength(qsizetype) const
 */
qsizetype binaryPlist::objLength(qsizetype index) const {

    auto objIndex = m_objectIndexTable[index];
    qsizetype size = 0;
    for (qsizetype i = objIndex.first; i < objIndex.second; ++i) {
        size += element_size(m_rawTable[i]);
    }
    return size;
}

/**
 * \file
 * * \copybrief binaryPlist::addDictonaryObject(const QList<quint64>&)
 */
void binaryPlist::addDictonaryObject(const QList<quint64> &l) {
    qsizetype start = m_rawTable.size();
    qsizetype pairNum = l.size() / 2;
    m_rawTable += { 0xD0 | (pairNum >= 0x0F ? 0x0F : pairNum ), CNTRL };
    if (pairNum >= 0x0F) {
        addIntegerObject(pairNum, false);
    }
    for (quint64 a : l) {
        m_rawTable += { a, OBJREF };
    }
    qsizetype end = m_rawTable.size();
    m_objectIndexTable += { start, end };
}

/**
 * \file
 * * \copybrief binaryPlist::addArrayObject(const QList<quint64>&)
 */
void binaryPlist::addArrayObject(const QList<quint64> &l) {
    qsizetype start = m_rawTable.size();
    m_rawTable += { 0xA0 | (l.size() >= 0x0F ? 0x0F : l.size()), CNTRL };
    if (l.size() >= 0x0F) {
        addIntegerObject(l.size(), false);
    }
    for (quint8 a : l) {
        m_rawTable += { a, OBJREF };
    }
    qsizetype end = m_rawTable.size();
    m_objectIndexTable += { start, end };
}

/**
 * \file
 * * \copybrief binaryPlist::addStringObject(const QString&)
 */
void binaryPlist::addStringObject(const QString &s) {
    qsizetype start = m_rawTable.size();
    m_rawTable += { 0x50 | (s.size() >= 0x0F ? 0x0F : s.size()), CNTRL };
    if (s.size() >= 0x0F) {
        addIntegerObject(s.size(), false);
    }
    addDataString(s);
    qsizetype end = m_rawTable.size();
    m_objectIndexTable += { start, end };
}


void binaryPlist::addUidObject(std::initializer_list<quint8> args) {
    qsizetype start = m_rawTable.size();
    m_rawTable += { 0x80 | (args.size() - 1), CNTRL };
    for (quint8 a : args) {
        m_rawTable += { a, DATA8 };
    }
    qsizetype end = m_rawTable.size();
    m_objectIndexTable += { start, end };
}

/**
 * \file
 * * \copybrief binaryPlist::finish(quint64)
 */
void binaryPlist::finish(quint64 topObject) {

    /**
     * Алгоритм:
     *
     * 1 Определить размер в байтах (1, 2, 4 или 8) числа m_objectRefSize, используемого
     * для хранения значений индексов объектов (элементов массивов и словарей)
     * в таблице объектов (Object Table) исходя из максимального значения --- то есть
     * последнего индекса таблицы индексов объектов m_objectIndexTable.
     */
    m_objectRefSize = 0;
    for (quint64 t = m_objectIndexTable.size() - 1; t != 0; t >>= 8) {
        m_objectRefSize++;
    }
    if (m_objectRefSize == 3) m_objectRefSize = 4;
    else if (m_objectRefSize > 4 && m_objectRefSize < 8) m_objectRefSize = 8;
    else if (m_objectRefSize > 8) {
       //!!! FATAL ERROR
    }
    qDebug() << "Исходя из количества объектов" << Qt::hex << Qt::showbase << m_objectIndexTable.size() << "получен размер индекса в байтах:" << m_objectRefSize;

    /**
     * 2 Определить размер в байтах (1, 2, 4 или 8) числа \ref binaryPlist::m_offsetIntSize "m_offsetIntSize", используемого
     * для хранения значения смещений в таблице объектов (Object Table) исходя из максимального
     * значения --- то есть смещения последнего элемента последнего объекта.
     */
    m_offsetIntSize = 0;
    for (quint64 t = m_rawTable.size() - 1; t != 0; t >>= 8) {
        m_offsetIntSize++;
    }
    if (m_offsetIntSize == 3) m_offsetIntSize = 4;
    else if (m_offsetIntSize > 4 && m_offsetIntSize < 8) m_offsetIntSize = 8;
    else if (m_offsetIntSize > 8) {
       //!!! FATAL ERROR
    }

    qDebug() << "Исходя из количества байтового размера таблицы объектов" << Qt::hex << Qt::showbase << m_rawTable.size() << "получен размер смещения в байтах:" << m_offsetIntSize;

    /**
     * 3 Дополнить хранилище двоичного представления списка таблицей смещений,
     * то есть значениями смещений каждого объекта:
     *
     * 3.1 Начальное значение текущего смещения (то есть смещение первого объекта) ---
     * это суммарный размер элементов двоичного представления от начала до первого объекта;
     */
    quint64 objOff = 0;
    for (qsizetype i = 0; i < m_objectIndexTable[0].first; ++i) {
        objOff += element_size(m_rawTable[i]);
    }
    /**
     * 3.2 После сохранения смещения каждого объекта текущее смещение увеличивается
     * на суммарный размер элементов этого объекта;
     */
    for (qsizetype i = 0; i < m_objectIndexTable.size(); ++i) {
        m_rawTable += { objOff, OBJOFF };
        objOff += objLength(i);
    }

    /**
     * 4 Дополнить хранилище двоичного представления списка эпилогом (Trailer)
     *
     * 4.1 добавить пятибайтовый массив unused: не используется, должно быть установлено нулевое значение
     */
    m_rawTable += { 0, DATA8 };
    m_rawTable += { 0, DATA8 };
    m_rawTable += { 0, DATA8 };
    m_rawTable += { 0, DATA8 };
    m_rawTable += { 0, DATA8 };

    /**
     * 4.2 Добавить байтовое число sortVersion: должно быть установлено нулевое значение
     */
    m_rawTable += { 0, DATA8 };

    /**
     * 4.3 Добавить байтовое число m_offsetIntSize: размер в байтах числа, используемого для хранения значения смещений
     * в таблице объектов (Object Table)
     */
    m_rawTable += { m_offsetIntSize, DATA8 };

    /**
     * 4.4 Добавить восьмибайтовое число m_objectRefSize: размер в байтах числа, используемого для хранения значения индексов
     * элементов массивов и словарей, используемых в таблице смещений (Offset Table)
     */
    m_rawTable += { m_objectRefSize, DATA8 };

    /**
     * 4.5 Добавить восьмибайтовое число, равное количеству записей в таблице смещений (Offset Table)
     */
    m_rawTable += { m_objectIndexTable.size(), DATA64 };

    /**
     * 4.6 Добавить восьмибайтовое число, равное индексу корневого объекта Plist
     */
    m_rawTable += { topObject, DATA64 };

    /**
     * 4.7 Добавить восьмибайтовое число, равное началу в двоичном представлении списка
     * таблицы смещений (Offset Table), или окончанию таблицы объектов (Objects Table).
     */
    m_rawTable += { objOff, DATA64 };
}

/**
 * \file
 * * \copybrief binaryPlist::parse_xml(QXmlStreamReader&, const QString&)
 */
binaryPlist::pNode *binaryPlist::parse_xml(QXmlStreamReader &xml, const QString &tag) {
    binaryPlist::pNode *root = nullptr;

    if (tag == "") {
        root = new pRoot(this);
    } else if (tag == "plist") {
        root = new pPlist(this);
    } else if (tag == "array") {
        root = new pArray(this);
    } else if (tag == "dict") {
        root = new pDict(this);
    } else if (tag == "integer") {
        root = new pInt(this);
    } else if (tag == "string") {
        root = new pString(this);
    } else if (tag == "key") {
        root = new pKey(this);
    } else {
        qCritical().noquote().nospace() << "Unsupported tag '" << tag << "'";
        return nullptr;
    }

    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        switch(token) {
            case QXmlStreamReader::NoToken:
                qCritical().noquote().nospace() << "XML error in '"<< root->tagToString() << "': NoToken state";
                return nullptr;
            case QXmlStreamReader::Invalid:
                qCritical().noquote().nospace() << "XML error in '"<< root->tagToString() << "': " << xml.errorString();
                return nullptr;
            case QXmlStreamReader::StartDocument:
                if (root->getTag() != tag_root) {
                    qCritical().noquote().nospace() << "XML error in '"<< root->tagToString() << "': StartDocument element";
                    return nullptr;
                }
                break;
            case QXmlStreamReader::EndDocument:
                if (root->getTag() != tag_root) {
                    qCritical().noquote().nospace() << "XML error in '"<< root->tagToString() << "': EndDocument element";
                    return nullptr;
                }
                break;
            case QXmlStreamReader::StartElement:
                {
                    pNode *node = parse_xml(xml, xml.name().toString());
                    if (node == nullptr) {
                        return nullptr;
                    }
                    root->addNode(node);
                }
                break;
            case QXmlStreamReader::EndElement:
                if (xml.name().toString() != root->tagToString()) {
                    delete root;
                    qCritical().noquote().nospace() << "XML error in '"<< root->tagToString() << "': not '"<< root->tagToString() << "' closing tag";
                    return nullptr;
                }

                /** При необходимости преобразовать dictonary в UID */
                if (root->isUid()) {
                    pUid *uid_element = new pUid(this);
                    uid_element->setIntValue(root->getUid());
                    delete root;
                    return uid_element;
                }
                if (root->getTag() != tag_root) {
                    root->resort();
                    return root;
                 }
                 break;
            case QXmlStreamReader::Characters:
                {
                    if (!xml.isWhitespace()) {
                        bool ok;
                        quint64 val = xml.text().toLongLong(&ok, 10);
                        if (ok) {
                            root->setIntValue(val);
                        }
                        root->setStringValue(xml.text().toString());
                    }
                }
                break;
            case QXmlStreamReader::Comment:
                break;
            case QXmlStreamReader::DTD:
                if (root->getTag() != tag_root) {
                    qCritical().noquote().nospace() << "XML error in '"<< root->tagToString() << "': DTD element";
                    return nullptr;
                 }
                 break;
            case QXmlStreamReader::EntityReference:
                break;
            case QXmlStreamReader::ProcessingInstruction:
                break;
        }
    }

    if (root->getTag() == tag_root) {
        root->findRef();
    }
    return root;
}

/**
 * \file
 * Функции, являющиеся методами класса \ref binaryPlist::pNode "pNode":
 * <BR>
 */

/**
 * \file
 * * \copybrief binaryPlist::pNode::pNode(enum binaryPlist::PLIST_TAG, class binaryPlist*)
 */
binaryPlist::pNode::pNode(enum binaryPlist::PLIST_TAG tag, class binaryPlist *parent) :
  m_parent(parent), m_tag(tag), m_alias(nullptr) {
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::~pNode()
 */
binaryPlist::pNode::~pNode() {
    for (auto i = 0; i < m_nodeList.size(); ++i) delete m_nodeList[i];
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::getParent() const
 */
binaryPlist *binaryPlist::pNode::getParent() const {
    return m_parent;
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::getTag() const
 */
enum binaryPlist::PLIST_TAG binaryPlist::pNode::getTag() const {
    return m_tag;
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::getAlias() const
 */
const binaryPlist::pNode *binaryPlist::pNode::getAlias() const {
    return m_alias;
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::setAlias(const binaryPlist::pNode*) const
 */
void binaryPlist::pNode::setAlias(const binaryPlist::pNode *alias) const {
    m_alias = alias;
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::getNodeList()
 */
const QList<binaryPlist::pNode*> &binaryPlist::pNode::getNodeList() const {
    return m_nodeList;
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::setNodeList(const QList<binaryPlist::pNode*>&)
 */
void binaryPlist::pNode::setNodeList(const QList<pNode*> &nodeList) {
    m_nodeList = nodeList;
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::addNode(binaryPlist::pNode*)
 */
void binaryPlist::pNode::addNode(binaryPlist::pNode *node) {
    m_nodeList += node;
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::isUid() const
 */
bool binaryPlist::pNode::isUid() const {
    if (m_nodeList.size() != 2) return false;
    if (m_nodeList[0]->getTag() != tag_key) return false;
    if (m_nodeList[1]->getTag() != tag_int) return false;
    if (m_nodeList[0]->getStringValue() != "CF$UID") return false;
    return true;
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::getUid() const
 */
quint64 binaryPlist::pNode::getUid() const {
    if (isUid()) return m_nodeList[1]->getIntValue();
    return 0;
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::resort()
 */
void binaryPlist::pNode::resort() {
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::toString() const
 */
QString binaryPlist::pNode::toString() const {
    QString s("<" + tagToString() + ">" + getStringValue());
    if (m_nodeList.size()) s += "\n";
    for (auto i = 0; i < m_nodeList.size(); ++i) {
        s += m_nodeList[i]->toString();
    }
    return s + "</" + tagToString() + ">\n";
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::findRef() const
 */
void binaryPlist::pNode::findRef() const {
    if (m_tag == tag_root) {
        m_parent->m_binary_list.clear();
    } else if (m_tag != tag_plist) {
        m_parent->m_binary_list += this;
    }
    for (auto i = 0; i < m_nodeList.size(); ++i) {
        m_nodeList[i]->findRef();
    }
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::setStringValue(const QString&)
 */
void binaryPlist::pNode::setStringValue(const QString &value) {
    Q_UNUSED(value);
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::getStringValue() const
 */
QString binaryPlist::pNode::getStringValue() const {
    return "";
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::setIntValue(quint64)
 */
void binaryPlist::pNode::setIntValue(quint64 value) {
    Q_UNUSED(value);
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::getIntValue() const
 */
quint64 binaryPlist::pNode::getIntValue() const {
    return -1;
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::getRefList() const
 */
QList<quint64> binaryPlist::pNode::getRefList() const {
    QList<quint64> l;

    for (auto i = 0; i < m_nodeList.size(); ++i) {
        l += m_nodeList[i]->getRef();
    }
    return l;
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::tagToString() const
 */
QString binaryPlist::pNode::tagToString() const {
    switch(m_tag) {
        case tag_root:
            return "root";
        case tag_plist:
            return "plist";
        case tag_bool:
            return "bool";
        case tag_fill:
            return "fill";
        case tag_int:
            return "integer";
        case tag_real:
            return "real";
        case tag_date:
            return "date";
        case tag_data:
            return "data";
        case tag_string:
            return "string";
        case tag_key:
            return "key";
        case tag_uid:
            return "uid";
        case tag_array:
            return "array";
        case tag_dict:
            return "dict";
        default:
            return "WRONG TAG";
    }
}

/**
 * \file
 * * \copybrief binaryPlist::pNode::getRef() const
 */
qsizetype binaryPlist::pNode::getRef() const {
    for (auto i = 0; i < m_parent->m_binary_list.size(); ++i) {
        if (this == m_parent->m_binary_list[i] || this->m_alias == m_parent->m_binary_list[i]) return i;
    }
    qCritical() << "CAN NOT FIND" << tagToString() << getStringValue();
    return -1;
}

/**
 * \file
 * Функции, являющиеся методами класса \ref binaryPlist::pInt "pInt":
 * <BR>
 */

/**
 * \file
 * * \copybrief binaryPlist::pInt::pInt()
 */
binaryPlist::pInt::pInt(class binaryPlist *parent, enum PLIST_TAG tag) :
  pNode(tag, parent), m_value(0) {
}

/**
 * \file
 * * \copybrief binaryPlist::pInt::setIntValue(quint64)
 */
void binaryPlist::pInt::setIntValue(quint64 value) {
    m_value = value;
}

/**
 * \file
 * * \copybrief binaryPlist::pInt::getIntValue() const
 */
quint64 binaryPlist::pInt::getIntValue() const {
    return m_value;
}

/**
 * \file
 * * \copybrief binaryPlist::pInt::getStringValue() const
 */
QString binaryPlist::pInt::getStringValue() const {
    return QString::number(m_value);
}

/**
 * \file
 * Функции, являющиеся методами класса \ref binaryPlist::pUid "pUid":
 * <BR>
 */

/**
 * \file
 * * \copybrief binaryPlist::pUid::pUid()
 */
binaryPlist::pUid::pUid(binaryPlist *parent) :
  pInt(parent, tag_uid) {
}

/**
 * \file
 * * \copybrief binaryPlist::pUid::toString() const
 */
QString binaryPlist::pUid::toString() const {
    return "<dict><key><CF$UID></key><integer>" + getStringValue() + "</integer></dict>\n";
}

/**
 * \file
 * * \copybrief binaryPlist::pUid::findRef() const
 */
void binaryPlist::pUid::findRef() const {
    for (auto i = 0; i < getParent()->m_binary_list.size(); ++i) {
        if (getParent()->m_binary_list[i]->getTag() == getTag() && 
          getParent()->m_binary_list[i]->getIntValue() == getIntValue()) {
             this->setAlias(getParent()->m_binary_list[i]);
             break;
        }
    }

    /**
     * \attention Воспроизведена оригинальная ошибка: независимо от найденного
     * алиаса объект добавляется в список.
     */
    getParent()->m_binary_list += this;
}

/**
 * \file
 * Функции, являющиеся методами класса \ref binaryPlist::pString "pString":
 * <BR>
 */

/**
 * \file
 * * \copybrief binaryPlist::pString::pString(binaryPlist *, enum binaryPlist::PLIST_TAG)
 */
binaryPlist::pString::pString(binaryPlist *parent, enum PLIST_TAG tag) :
  pNode(tag, parent) {
}

/**
 * \file
 * * \copybrief binaryPlist::pString::findRef() const
 */
void binaryPlist::pString::findRef() const {
    for (auto i = 0; i < getParent()->m_binary_list.size(); ++i) {
        if (getParent()->m_binary_list[i]->getTag() == getTag() && 
          getParent()->m_binary_list[i]->getStringValue() == m_value) {
             this->setAlias(getParent()->m_binary_list[i]);
             break;
        }
    }
    if (getAlias() == nullptr) {
        getParent()->m_binary_list += this;
    }
}

/**
 * \file
 * * \copybrief binaryPlist::pString::setStringValue(const QString&)
 */
void binaryPlist::pString::setStringValue(const QString &value) {
    m_value = value;
}

/**
 * \file
 * * \copybrief binaryPlist::pString::getStringValue() const
 */
QString binaryPlist::pString::getStringValue() const {
    return m_value;
}

/**
 * \file
 * Функции, являющиеся методами класса \ref binaryPlist::pKey "pKey":
 * <BR>
 */

/**
 * \file
 * * \copybrief binaryPlist::pKey::pKey()
 */
binaryPlist::pKey::pKey(binaryPlist *parent) :
  pString(parent, tag_key) {
}

/**
 * \file
 * Функции, являющиеся методами класса \ref binaryPlist::pArray "pArray":
 * <BR>
 */

/**
 * \file
 * * \copybrief binaryPlist::pArray::pArray(binaryPlist*, enum binaryPlist::PLIST_TAG)
 */
binaryPlist::pArray::pArray(binaryPlist *parent, enum PLIST_TAG tag) :
  pNode(tag, parent) {
}

/**
 * \file
 * Функции, являющиеся методами класса \ref binaryPlist::pDict "pDict":
 * <BR>
 */

/**
 * \file
 * * \copybrief binaryPlist::pDict::pDict(binaryPlist*)
 */
binaryPlist::pDict::pDict(binaryPlist *parent) :
  pArray(parent, tag_dict) {
}

/**
 * \file
 * * \copybrief binaryPlist::pDict::resort()
 */
void binaryPlist::pDict::resort() {
    QList<pNode*> newList;
    for (auto i = 0; i < getNodeList().size() / 2; ++i) {
        newList += getNodeList()[i * 2 + 0];
    }
    for (auto i = 0; i < getNodeList().size() / 2; ++i) {
        newList += getNodeList()[i * 2 + 1];
    }
    setNodeList(newList);
}

/**
 * \file
 * Функции, являющиеся методами класса \ref binaryPlist::pPlist "pPlist":
 * <BR>
 */

/**
 * \file
 * * \copybrief binaryPlist::pPlist::pPlist(binaryPlist*)
 */
binaryPlist::pPlist::pPlist(binaryPlist *parent) :
  pArray(parent, tag_plist) {
}

/**
 * \file
 * Функции, являющиеся методами класса \ref binaryPlist::pRoot "pRoot":
 * <BR>
 */

/**
 * \file
 * * \copybrief binaryPlist::pRoot::pRoot(binaryPlist*)
 */
binaryPlist::pRoot::pRoot(binaryPlist *parent) :
  pArray(parent, tag_root) {
}
