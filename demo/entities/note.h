#ifndef EG_DEMO_NOTE_H
#define EG_DEMO_NOTE_H

// a musical note representing sound produced by a musical instrument

#include "example.h"

/**
 * Populates an entity type struct with information about the note entity.
 *
 * Params:
 *   eg_entity_type* - the note's entity type definition
 */
void note_demo_register(eg_entity_type *);

/**
 * Creates a note entity.
 *
 * Params:
 *   int - the note entity's initial x position
 *   int - the note entity's initial y position
 *
 * Returns:
 *   eg_entity* - a pointer to the new note entity.
 */
eg_entity *note_demo_create(eg_app *, int, int);

#endif