#ifndef EG_TNS_NOTE_H
#define EG_TNS_NOTE_H

// A note represents the sound produced by a musical instrument.

#include "example.h"

/**
 * Populates an entity type struct with information about the note entity.
 *
 * Params:
 *   eg_entity_type* - the note's entity type definition
 */
void tns_register_note(eg_entity_type *);

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
eg_entity *tns_create_note(eg_app *, int, int);

#endif