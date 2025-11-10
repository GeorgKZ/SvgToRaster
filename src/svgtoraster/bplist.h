/**
 * \file
 * \brief Заголовочный файл с объявлением класса \ref binaryPlist "binaryPlist"
 *
 * Класс предназначен для преобразования списка в представлении XML <a href="https://en.wikipedia.org/wiki/Property_list">Plist</a>
 * (формат version="1.0"), принятого в <a href="https://ru.wikipedia.org/wiki/MacOS">macOS</a>,
 * в двоичное представление (формат v"0?")
 */
#ifndef BPLIST_H
#define BPLIST_H

#include <initializer_list>
#include <QList>
#include <QPair>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>

/**
 * \brief Класс для преобразования списка в представлении XML <a href="https://en.wikipedia.org/wiki/Property_list">Plist</a>
 * (формат version="1.0"), принятого в <a href="https://ru.wikipedia.org/wiki/MacOS">macOS</a>,
 * в двоичное представление (формат v"0?")
 */
class binaryPlist {

    public:

    /**
     * \brief Конструктор
     */
    binaryPlist();

    /**
     * \brief Парсер и конвертер к двоичному виду
     */
    void parser(const QString &plist);

    /**
     * \brief Получить размер в байтах двоичного представления списка
     */
    qsizetype size() const;

    /**
     * \brief Вывести двоичное представление списка в поток
     */
    void output(QDataStream &out) const;

    private:

    /**
     * \brief Размер в байтах (1, 2, 4 или 8) чисел, используемых для хранения значений смещений
     * в таблице объектов (Object Table) и образующих таблицу смещений (Offset Table)
     */
    qsizetype m_offsetIntSize;

    /**
     * \brief Размер в байтах (степень числа 2) чисел, используемых для хранения значений индексов
     * объектов (элементов массивов и словарей) в таблице объектов (Object Table)
     */
    qsizetype m_objectRefSize;

    /**
     * \brief Список пар, соответствующих индексам первого и следующего за последним
     * элементов таблицы двоичного представления списка, определяющих объекты
     */
    QList<QPair<qsizetype,qsizetype>> m_objectIndexTable;

    /**
     * \brief Перечисление возможных типов объектов списка Plist в промежуточном и
     * двоичном представлении
     */
    enum PLIST_TAG {

        /**
         * Тип для корневого объекта списка Plist
         */
        tag_root,

        /**
         * Тип для объекта с тегом "plist"
         */
        tag_plist,

        /**
         * Тип для булевого значения с тегами "true" и "false"
         */
        tag_bool,

        /**
         * Тип для выравнивающего байта в двоичном представлении списка
         */
        tag_fill,

        /**
         * Тип для целочисленного числа с тегом "integer"
         */
        tag_int,

        /**
         * Тип для действительного числа с тегом "real"
         * \todo Сделать этот тип.
         */
        tag_real,

        /**
         * Тип для даты с тегом "date"
         * \todo Сделать этот тип.
         */
        tag_date,

        /**
         * Тип для двоичных данных с тегом "data"
         * \todo Сделать этот тип.
         */
        tag_data,

        /**
         * Тип для строк с тегом "string",
         * в двоичном представлении для хранения однобайтных символов ASCII
         */
        tag_string,

        /**
         * Тип для строк с тегом "string",
         * в двоичном представлении для хранения двухбайтных символов
         * (первым идёт старший байт)
         * \todo Сделать этот тип.
         */
        tag_ustring,

        /**
         * Тип для ключей в составе словарей с тегом "key"
         */
        tag_key,

        /**
         * Тип для хранения в двоичном представлении беззнаковых
         * целых чисел, представленных в XML-представлении как словарь со значением
         * ключа "CF$UID" и целым числом
         */
        tag_uid,

        /**
         * Тип для массивов с тегом "array"
         */
        tag_array,

        /**
         * Тип для словарей с тегом "dict"
         */
        tag_dict
    };

    /**
     * \brief Перечисление возможных типов элементов переменного размера в двоичном
     * представлении списка
     */
    enum TYPE {

        /**
         * Контрольный байт, определяющий тип объекта
         */
        CNTRL,

        /**
         * Байт данных
         */
        DATA8,

        /**
         * Восьмибайтовое число
         */
        DATA64,

        /**
         * Индекс объекта или ключа размером \ref binaryPlist::m_objectRefSize "m_objectRefSize" байт
         */
        OBJREF,

        /**
         * Смещение объекта в таблице объектов размером \ref binaryPlist::m_offsetIntSize "m_offsetIntSize" байт
         */
        OBJOFF
    };

    /**
     * \brief Хранилище двоичного представления списка в виде списка элементов
     * переменного размера
     */
    QList<QPair<qsizetype,enum TYPE>> m_rawTable;

    /**
     * \brief Получить размер в байтах элемента переменного размера
     *
     * \note Для правильной работы функции должно быть определено значение \ref binaryPlist::m_objectRefSize "m_objectRefSize".
     * \note Для правильной работы функции для элементов, расположенных после таблицы объектов,
     * также должно быть определено значение \ref binaryPlist::m_offsetIntSize "m_offsetIntSize".
     *
     * \param [in] element элемент двоичного представления, размер которого требуется определить
     * \return размер в байтах элемента переменного размера.
     */
    qsizetype element_size(const QPair<qsizetype, enum TYPE> &element) const;

    /**
     * \brief Дополнить двоичное представлене списка байтами указанной строки
     */
    void addDataString(const QString& s);

    /**
     * \brief Дополнить двоичное представлене списка байтами указанного массива
     */
    void addByteArray(const QByteArray& b);

    /**
     * \brief Получить размер двоичного представления объекта в двоичном представлении списка
     * \note Для правильной работы функции должно быть определено значение \ref binaryPlist::m_objectRefSize "m_objectRefSize".
     *
     * \param [in] index индекс объекта, размер которого требуется определить
     */
    qsizetype objLength(qsizetype index) const;

    /**
     * \brief Добавление заполняющего байта в двоичное представление списка
     */
    void addFillObject();

    /**
     * \brief Добавление булева значения в форме двоичного объекта
     * в двоичное представление списка
     */
    void addBooleanObject(bool val);

    /**
     * \brief Добавление целого числа (1, 2, 4, 8 байт длиной) в форме двоичного объекта
     * в двоичное представление списка
     */
    void addIntegerObject(quint64 val, bool isObject = true);

    /**
     * \brief Добавление действительного числа (4 или 8 байт длиной) в форме двоичного объекта
     * в двоичное представление списка
     */
    void addRealObject(qreal val);

    /**
     * \brief Добавление словаря со списком номеров объектов в форме двоичного объекта
     * в двоичное представление списка
     */
    void addDictonaryObject(const QList<quint64> &l);

    /**
     * \brief Добавление массива со списком номеров объектов в форме двоичного объекта
     * в двоичное представление списка
     */
    void addArrayObject(const QList<quint64> &l);

    /**
     * \brief Добавление последовательности байт, закодированных по алгоритму <a href="https://ru.wikipedia.org/wiki/Base64">Base-64</a>,
     * в двоичное представление списка
     */
    void addDataObject(const QString &b);

    /**
     * \brief Добавление даты, указанной в формате <a href="https://en.wikipedia.org/wiki/ISO_8601">ISO 8601</a>,
     * в двоичное представление списка
     */
    void addDateObject(const QString &d);

    /**
     * \brief Проверить, входит ли символ в подмножество ASCII
     */
    static bool isASCII(QChar c);

    /**
     * \brief Добавление строки в форме двоичного объекта
     * в двоичное представление списка
     */
    void addStringObject(const QString &s);

    /**
     * \brief Добавление UID в форме двоичного объекта
     * в двоичное представление списка
     */
    void addUidObject(quint64 val);

    /**
     * \brief Добавление таблицы смещений и трейлера
     */
    void finish(quint64 topObject);

    /**
     * \brief Вспомогательный абстрактный класс для промежуточного представления
     * объектов из разобранного XML-представления перед преобразованием в двоичное представление
     */
    class pNode {

        private:

        /**
         * \brief Указатель на родительский класс \ref binaryPlist "binaryPlist"
         */
        binaryPlist *m_parent;

        /**
         * \brief Тег, определённый через \ref binaryPlist::PLIST_TAG "enum PLIST_TAG"
         */
        enum PLIST_TAG m_tag;

        /**
         * \brief Указатель на класс с таким же содержанием
         */
        mutable const pNode *m_alias;

        /** \brief Список объектов, входящих в этот объект */
        QList<pNode*> m_nodeList;

        public:

        /**
         * \brief Конструктор
         */
        pNode(enum PLIST_TAG tag, class binaryPlist *parent);

        /**
         * \brief Деструктор
         */
        virtual ~pNode();

        /**
         * \brief Получить указатель на родительский класс \ref binaryPlist "binaryPlist"
         */
        binaryPlist *getParent() const;

        /**
         * \brief Получить тип тега объекта
         */
        enum PLIST_TAG getTag() const;

        /**
         * \brief Получить указатель на идентичный объект-заместитель
         */
        const pNode *getAlias() const;

        /**
         * \brief Установить указатель на идентичный объект-заместитель
         */
        void setAlias(const pNode *alias) const;

        /**
         * \brief Получить список узлов в составе данного узла
         */
        const QList<pNode*> &getNodeList() const;

        /**
         * \brief Установить список узлов в составе данного узла
         */
        void setNodeList(const QList<pNode*> &nodeList);

        /**
         * \brief Добавить узел в список узлов в составе данного узла
         */
        void addNode(pNode* node);

        /**
         * \brief Проверить, является ли этот объект объектом UID
         */
        bool isUid() const;

        /**
         * \brief Получить числовое значение объекта UID
         */
        quint64 getUid() const;

        /**
         * \brief Выполнить перестановку из "список ключ/значение" к "все ключи/все значения"
         */
        virtual void resort();

        /**
         * \brief Получить строковое XML-представление объекта
         */
        virtual QString toString() const;

        /**
         * \brief Добавить список узлов данного узла и данный узел к "плоскому" списку узлов
         */
        virtual void findRef() const;

        /**
         * \brief Установить строковое значение узла
         */
        virtual void setStringValue(const QString &value);

        /**
         * \brief Получить строковое значение узла
         */
        virtual QString getStringValue() const;

        /**
         * \brief Установить целочисленное значение узла
         */
        virtual void setIntValue(quint64 value);

        /**
         * \brief Получить целочисленное значение узла
         */
        virtual quint64 getIntValue() const;

        /**
         * \brief Установить числовое (действительное) значение узла
         */
        virtual void setRealValue(double value);

        /**
         * \brief Получить числовое (действительное) значение узла
         */
        virtual double getRealValue() const;

        /**
         * \brief Установить булево значение узла
         */
        virtual void setBoolValue(bool value);

        /**
         * \brief Получить булево значение узла
         */
        virtual bool getBoolValue() const;

        /**
         * \brief Получить список узлов данного узла
         */
        virtual QList<quint64> getRefList() const;

        /**
         * \brief Получить строковое представление типа узла
         */
        QString tagToString() const;

        /**
         * \brief Получить индекс данного узла в "плоском" списке узлов
         */
        qsizetype getRef() const;

        /**
         * \brief Проверить, совместим ли данный узел с указанным тегом XML-представления
         */
        bool checkTag(const QString& tag) const;
    };

    /**
     * \brief "Плоский" список всех объектов в том порядке, в каком они будут
     * записаны в двоичный список
     */
    QList<const pNode*> m_binary_list;

    /**
     * \brief Разбор XML-представления списка и формирование списка объектов в
     * виде классов, дочерних по отношению к \ref binaryPlist::pNode "pNode"
     */
    pNode *parse_xml(QXmlStreamReader &xml);

    /**
     * \brief Вспомогательный класс для промежуточного представления
     * булевых значений из разобранного XML-представления
     * перед преобразованием в двоичное представление
     */
    class pBool : public pNode {

        /** \brief Булево значение */
        bool m_value;

        public:

        /**
         * \brief Конструктор
         */
        pBool(class binaryPlist *parent, enum PLIST_TAG tag = tag_bool);

        /**
         * \brief Установить булево значение
         */
        virtual void setBoolValue(bool value) override;

        /**
         * \brief Получить числовое значение
         */
        virtual bool getBoolValue() const override;

        /**
         * \brief Получить строковое значение
         */
        virtual QString getStringValue() const override;
    };

    /**
     * \brief Вспомогательный класс для промежуточного представления
     * целых чисел из разобранного XML-представления
     * перед преобразованием в двоичное представление
     */
    class pInt : public pNode {

        /** \brief Целочисленное значение */
        quint64 m_value;

        public:

        /**
         * \brief Конструктор
         */
        pInt(class binaryPlist *parent, enum PLIST_TAG tag = tag_int);

        /**
         * \brief Установить числовое значение
         */
        virtual void setIntValue(quint64 value) override;

        /**
         * \brief Получить числовое значение
         */
        virtual quint64 getIntValue() const override;

        /**
         * \brief Получить строковое значение
         */
        virtual QString getStringValue() const override;
    };

    /**
     * \brief Вспомогательный класс для промежуточного представления
     * действительных чисел из разобранного XML-представления
     * перед преобразованием в двоичное представление
     */
    class pReal : public pNode {

        /** \brief Действительное значение */
        double m_value;

        public:

        /**
         * \brief Конструктор
         */
        pReal(class binaryPlist *parent);

        /**
         * \brief Установить числовое (действительное) значение узла
         */
        virtual void setRealValue(double value) override;

        /**
         * \brief Получить числовое (действительное) значение узла
         */
        virtual double getRealValue() const override;

        /**
         * \brief Получить строковое значение
         */
        virtual QString getStringValue() const override;
    };

    /**
     * \brief Вспомогательный класс для промежуточного представления
     * идентификаторв UID из разобранного XML-представления
     * перед преобразованием в двоичное представление
     */
    class pUid : public pInt {

        public:

        /**
         * \brief Конструктор
         */
        pUid(binaryPlist *parent);

        /**
         * \brief Получить строковое XML-представление объекта
         */
        virtual QString toString() const override;

        /**
         * \brief Добавить список узлов данного узла и данный узел к "плоскому" списку узлов
         */
        virtual void findRef() const override;
    };

    /**
     * \brief Вспомогательный класс для промежуточного представления
     * строк из разобранного XML-представления
     * перед преобразованием в двоичное представление
     */
    class pString : public pNode {

        private:

        /**
         * \brief Значение строки
         */
        QString m_value;

        public:

        /**
         * \brief Конструктор
         */
        pString(binaryPlist *parent, enum PLIST_TAG tag = tag_string);

        /**
         * \brief Добавить список узлов данного узла и данный узел к "плоскому" списку узлов
         */
        virtual void findRef() const override;

        /**
         * \brief Установить строковое значение узла
         */
        virtual void setStringValue(const QString &value) override;

        /**
         * \brief Получить строковое значение
         */
        virtual QString getStringValue() const override;
    };

    /**
     * \brief Вспомогательный класс для промежуточного представления
     * ключей словарей из разобранного XML-представления
     * перед преобразованием в двоичное представление
     */
    class pKey : public pString {

        public:

        /**
         * \brief Конструктор
         */
        pKey(binaryPlist *parent);
    };

    /**
     * \brief Вспомогательный класс для промежуточного представления
     * последовательности байт (хранимой в виде строки в кодировке <a href="https://ru.wikipedia.org/wiki/Base64">Base-64</a>)
     * из разобранного XML-представления перед преобразованием в двоичное представление
     */
    class pData : public pString {

        public:

        /**
         * \brief Конструктор
         */
        pData(binaryPlist *parent);
    };

    /**
     * \brief Вспомогательный класс для промежуточного представления
     * даты из разобранного XML-представления перед преобразованием в двоичное представление
     */
    class pDate : public pString {

        public:

        /**
         * \brief Конструктор
         */
        pDate(binaryPlist *parent);
    };

    /**
     * \brief Вспомогательный класс для промежуточного представления
     * массивов из разобранного XML-представления
     * перед преобразованием в двоичное представление
     */
    class pArray : public pNode {

        public:

        /**
         * \brief Конструктор
         */
        pArray(binaryPlist *parent, enum PLIST_TAG tag = tag_array);
    };

    /**
     * \brief Вспомогательный класс для промежуточного представления
     * словарей (тег "dict") из разобранного XML-представления
     * перед преобразованием в двоичное представление
     */
    class pDict : public pArray {

        public:

        /**
         * \brief Конструктор
         */
        pDict(binaryPlist *parent);

        virtual void resort() override;
    };

    /**
     * \brief Вспомогательный класс для промежуточного представления
     * тега "plist" из разобранного XML-представления
     * перед преобразованием в двоичное представление
     */
    class pPlist : public pArray {

        public:

        /**
         * \brief Конструктор
         */
        pPlist(binaryPlist *parent);
    };


    /**
     * \brief Вспомогательный класс для промежуточного представления
     * списка Plist ключей словарей из разобранного XML-представления
     * перед преобразованием в двоичное представление
     */
    class pRoot : public pArray {

        public:

        /**
         * \brief Конструктор
         */
        pRoot(binaryPlist *parent);
    };
};

#endif // BPLIST_H
