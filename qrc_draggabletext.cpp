/****************************************************************************
** Resource object code
**
** Created: Fri Oct 22 21:26:59 2010
**      by: The Resource Compiler for Qt version 4.6.3
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <QtCore/qglobal.h>

static const unsigned char qt_resource_data[] = {
  // /Users/jennmat/Workspace/QT/DnD/words.txt
  0x0,0x0,0x0,0x22,
  0x74,
  0x65,0x73,0x74,0x20,0x74,0x68,0x69,0x73,0x20,0x69,0x73,0x20,0x61,0x20,0x74,0x65,
  0x73,0x74,0x20,0x6f,0x66,0x20,0x64,0x72,0x61,0x67,0x67,0x61,0x62,0x6c,0x65,0xa,
  0xa,
  
};

static const unsigned char qt_resource_name[] = {
  // dictionary
  0x0,0xa,
  0xb,0xb,0x17,0xd9,
  0x0,0x64,
  0x0,0x69,0x0,0x63,0x0,0x74,0x0,0x69,0x0,0x6f,0x0,0x6e,0x0,0x61,0x0,0x72,0x0,0x79,
    // words.txt
  0x0,0x9,
  0x8,0xb6,0xa7,0x34,
  0x0,0x77,
  0x0,0x6f,0x0,0x72,0x0,0x64,0x0,0x73,0x0,0x2e,0x0,0x74,0x0,0x78,0x0,0x74,
  
};

static const unsigned char qt_resource_struct[] = {
  // :
  0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,
  // :/dictionary
  0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x2,
  // :/dictionary/words.txt
  0x0,0x0,0x0,0x1a,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x0,

};

QT_BEGIN_NAMESPACE

extern Q_CORE_EXPORT bool qRegisterResourceData
    (int, const unsigned char *, const unsigned char *, const unsigned char *);

extern Q_CORE_EXPORT bool qUnregisterResourceData
    (int, const unsigned char *, const unsigned char *, const unsigned char *);

QT_END_NAMESPACE


int QT_MANGLE_NAMESPACE(qInitResources_draggabletext)()
{
    QT_PREPEND_NAMESPACE(qRegisterResourceData)
        (0x01, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}

Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_draggabletext))

int QT_MANGLE_NAMESPACE(qCleanupResources_draggabletext)()
{
    QT_PREPEND_NAMESPACE(qUnregisterResourceData)
       (0x01, qt_resource_struct, qt_resource_name, qt_resource_data);
    return 1;
}

Q_DESTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qCleanupResources_draggabletext))

