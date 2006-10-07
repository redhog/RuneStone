#ifndef RUNE_STONE_OBJECT_H
#define RUNE_STONE_OBJECT_H

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

Dirt_Struct *RuneStone_Struct_classStruct(Dirt_Session *session, char **path, size_t paths);
Dirt_Struct *RuneStone_Struct_instanceStruct(Dirt_Session *session, Dirt_Struct *class, Dirt_Struct **baseValue, size_t baseValue, schar **name, Dirt_Struct **value, size_t members);

#endif
