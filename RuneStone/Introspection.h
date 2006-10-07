#ifndef RUNE_STONE_INTROSPECTION_H
#define RUNE_STONE_INTROSPECTION_H

#include <Dirt/Struct.h>

typedef struct RuneStone_Struct_AnnotatedTypeStructT struct RuneStone_Struct_AnnotatedTypeStruct;
Dirt_StructType RuneStone_StructType_AnnotatedType;
struct RuneStone_Struct_AnnotatedTypeStructT {
 Dirt_Struct base;
 Dirt_Struct *comment;
 Dirt_Struct *value;
}

typedef struct RuneStone_Struct_ParamsTypeStructT struct RuneStone_Struct_ParamsTypeStruct;
Dirt_StructType RuneStone_StructType_ParamsType;
struct RuneStone_Struct_ParamsTypeStructT {
 Dirt_Struct base;
 char **argname;
 Dirt_Struct **argtype;
 size_t args;
 int required;
 Dirt_Struct *resargstype;
 Dirt_Struct *reskwtype;
 Dirt_Struct *convertType;
}

Dirt_Struct *RuneStone_Struct_annotatedTypeStruct(Dirt_Session *session, Dirt_Struct *obj, Dirt_Struct *comment);
Dirt_Struct *RuneStone_Struct_paramsTypeStruct(Dirt_Session *session, char **argname, Dirt_Struct **argtype, size_t args, int required, Dirt_Struct *resargstype, Dirt_Struct *reskwtype,  Dirt_Struct *convertType);

#endif
