#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#define MAP_ALLOC_UNIT 3

struct mapStruct {
	void **keys;
	void **values;
	int max;
	int num;
	bool (*equals)(const void *a,const void *b);
};

typedef struct mapStruct *MapADT;
#define _MAP_IMPL_
#include "mapADT.h"

MapADT map_create( bool (*equals)(const void *a,const void *b) ){
	MapADT ret;
	ret = (MapADT)malloc(sizeof(struct mapStruct));
	if(ret!=NULL){
		ret->equals=equals;
		ret->num=0;
		ret->max=0;
		ret->keys=NULL;
		ret->values=NULL;
	}
	return ret;
}

void map_destroy( MapADT map ){
	if(map->keys!=NULL){
		free(map->keys);
	}
	if(map->values!=NULL){
		free(map->values);
	}
	free(map);
}

void map_clear( MapADT map ){
	if(map->keys!=NULL){
		free(map->keys);
		map->keys=NULL;
	}
	if(map->values!=NULL){
		free(map->values);
		map->values=NULL;
	}
	map->max=0;
	map->num=0;	
}

bool map_contains(MapADT map, void *key){
	int num = map->num;
	bool ret = 0;
	for(int i=0;i<num;i++){
		ret = map->equals(key, map->keys[i]);
		if(ret){
			return ret;
		}
	}
	return ret;
}

void* map_put( MapADT map, void *key, void *value){
	void* ret=NULL;
	if(map_empty(map)){
		map->keys = malloc(sizeof(void *)*MAP_ALLOC_UNIT);
		map->values = malloc(sizeof(void *)*MAP_ALLOC_UNIT);
		map->max=MAP_ALLOC_UNIT;
		map->keys[0]=key;
		map->values[0]=value;
		map->num++;
		return ret;
	}else{
		if(map_contains(map, key)){
			int n=map->num;
			for(int i=0;i<n;i++){
				if(map->equals(key, map->keys[i])){
					ret=map->values[i];
					map->values[i]=value;
					map->keys[i]=key;
					return ret;
				}
			}
		}else{
			if(map->num>=map->max){
				void *tempK;//swap to double star maybe
				void *tempV;
				tempK=realloc(map->keys,sizeof(void *)*
				(map->num+MAP_ALLOC_UNIT));
				tempV=realloc(map->values,sizeof(void *)*
				(map->num+MAP_ALLOC_UNIT));
				map->keys=tempK, map->values=tempV;
				map->max+=MAP_ALLOC_UNIT;
			}
			map->keys[map->num]=key;
			map->values[map->num]=value;
			map->num++;
			return ret;
		}
	}
	return ret;
}

const void *map_get( MapADT map, void *key ){
	assert(!map_empty(map));
	void *ret = NULL;
	int num=map->num;
	for(int i=0;i<num;i++){
		if(map->equals(key, map->keys[i])){
			ret = map->values[i];
		}
	}
	return ret;
}
//
//moves values and keys leftward in arrays to fill a gap left by removing
//a value from the map
//
//@param map map to be shifted
//@param n number of objects in map before the deletion
//
static void map_shift(MapADT map, int n){
	int placed = 0;
	void **tempK;
	void **tempV;
	tempK = malloc(sizeof(void *)*map->max);
	tempV = malloc(sizeof(void *)*map->max);
	for(int i=0;i<n;i++){
		if(map->keys[i]!=NULL){
			tempK[placed]=map->keys[i];
			tempV[placed]=map->values[i];
			placed++;
		}
	}
	free(map->keys);
	free(map->values);
	map->keys=tempK;
	map->values=tempV;
}

void *map_delete( MapADT map, void *key ){
	assert(!map_empty(map));
	assert(map_contains(map, key));
	void *ret=NULL;
	int num=map->num;
	for(int i=0;i<num;i++){
	assert(!(map->keys[i]==NULL));
		if(map->equals(key, map->keys[i])){
			ret = map->values[i];
			free(map->keys[i]);
			map->keys[i]=NULL;
			map->values[i]=NULL;
			map->num--;
			map_shift(map,num);
			return ret;
		}
	}
	return ret;
}

bool map_empty( MapADT map ){
	return (map->num==0);
}