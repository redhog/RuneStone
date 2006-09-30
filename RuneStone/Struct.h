#ifndef RUNE_STONE_STRUCT_H
#define RUNE_STONE_STRUCT_H

#include <Dirt/Struct.h>


typedef struct RuneStone_Struct_ClassStructT struct RuneStone_Struct_ClassStruct;
Dirt_StructType RuneStone_StructType_Class;
struct RuneStone_Struct_ClassStructT {
 Dirt_Struct base;
 char **path; /* Array of strings like "Grimoire.Types.ParamsType" */
 size_t paths;
}

typedef struct RuneStone_Struct_InstanceStructT struct RuneStone_Struct_InstanceStruct;
Dirt_StructType RuneStone_StructType_Instance;
struct RuneStone_Struct_InstanceStructT {
 Dirt_Struct base;
 Dirt_Struct *class;
 Dirt_Struct **baseValue;
 size_t baseValues;
 char **name;
 Dirt_Struct **value;
 size_t members;
}

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

Dirt_Struct *RuneStone_Struct_classStruct(Dirt_Session *session, char **path, size_t paths);
Dirt_Struct *RuneStone_Struct_instanceStruct(Dirt_Session *session, Dirt_Struct *class, Dirt_Struct **baseValue, size_t baseValue, schar **name, Dirt_Struct **value, size_t members);
Dirt_Struct *RuneStone_Struct_annotatedTypeStruct(Dirt_Session *session, Dirt_Struct *obj, Dirt_Struct *comment);
Dirt_Struct *RuneStone_Struct_paramsTypeStruct(Dirt_Session *session, char **argname, Dirt_Struct **argtype, size_t args, int required, Dirt_Struct *resargstype, Dirt_Struct *reskwtype,  Dirt_Struct *convertType);

#endif
