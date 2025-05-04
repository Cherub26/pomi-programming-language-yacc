#ifndef POMI_H
#define POMI_H

typedef enum { typeCon, typeId, typeOpr, typeStr, typeFloat, typeBool } nodeEnum;

/* constants */
typedef struct {
    int value;                  /* value of constant */
} conNodeType;

/* floating-point numbers */
typedef struct {
    float value;                /* value of float */
} floatNodeType;

/* boolean values */
typedef struct {
    bool value;                 /* boolean value */
} boolNodeType;

/* identifiers */
typedef struct {
    int i;                      /* subscript to sym array */
} idNodeType;

/* operators */
typedef struct {
    int oper;                   /* operator */
    int nops;                   /* number of operands */
    struct nodeTypeTag *op[1];  /* operands (expandable) */
} oprNodeType;

/* string constants */
typedef struct {
    char* value;                /* string value */
} strNodeType;

typedef struct nodeTypeTag {
    nodeEnum type;              /* type of node */
    union {
        conNodeType con;        /* constants */
        idNodeType id;          /* identifiers */
        oprNodeType opr;        /* operators */
        strNodeType str;        /* strings */
        floatNodeType flt;      /* floating-point numbers */
        boolNodeType boolean;   /* boolean values */
    };
} nodeType;

#endif
