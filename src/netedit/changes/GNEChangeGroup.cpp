/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2006-2021 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    GNEChangeGroup.cpp
/// @author  Pablo Alvarez Lopez
/// @date    Sep 2021
///
//
/****************************************************************************/

// ===========================================================================
// included modules
// ===========================================================================

#include "GNEChangeGroup.h"


// ===========================================================================
// FOX-declarations
// ===========================================================================


// Object implementation
FXIMPLEMENT(GNEChangeGroup, GNEChange, nullptr, 0)

// ---------------------------------------------------------------------------
// GNEChangeGroup - methods
// ---------------------------------------------------------------------------

GNEChangeGroup::GNEChangeGroup(const std::string &description) :
    undoList(nullptr), 
    redoList(nullptr), 
    group(nullptr),
    myDescription(description) {
}


GNEChangeGroup::~GNEChangeGroup() {
    register GNEChange *change;
    while (redoList) {
        change = redoList;
        redoList = redoList->next;
        delete change;
    }
    while (undoList) {
        change = undoList;
        undoList = undoList->next;
        delete change;
    }
    delete group;
}


const std::string&
GNEChangeGroup::getDescription() {
    return myDescription;
}


std::string
GNEChangeGroup::undoName() const {
    return ("Undo " + myDescription);
}


std::string
GNEChangeGroup::redoName() const {
    return ("Redo " + myDescription);
}


bool
GNEChangeGroup::empty() const { 
    return (undoList == nullptr); 
}


void
GNEChangeGroup::undo() {
    register GNEChange *change;
    while (undoList) {
        change = undoList;
        undoList = undoList->next;
        change->undo();
        change->next = redoList;
        redoList = change;
    }
}


void GNEChangeGroup::redo() {
    register GNEChange *change;
    while (redoList) {
        change = redoList;
        redoList = redoList->next;
        change->redo();
        change->next = undoList;
        undoList = change;
    }
}


int
GNEChangeGroup::size() const {
    register FXuint result = sizeof(GNEChangeGroup);
    register GNEChange *change;
    for (change = undoList; change; change = change->next) {
        result += change->size();
    }
    for (change = redoList; change; change = change->next) {
        result += change->size();
    }
    return result;
}


GNEChangeGroup::GNEChangeGroup() :
    undoList(nullptr), 
    redoList(nullptr), 
    group(nullptr) {
}