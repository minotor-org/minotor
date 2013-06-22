/*
 * Copyright 2012, 2013 Gauthier Legrand
 * Copyright 2012, 2013 Romuald Conty
 * 
 * This file is part of Minotor.
 * 
 * Minotor is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Minotor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with Minotor.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mipromatrix.h"

#include "minotor.h"

#include "minaflash.h"
#include "minarandompixels.h"
#include "minastars.h"
#include "minafallingobjects.h"
#include "minavibration.h"

// Program "Matrix": animation sets of green effects (Matrix-like)
MiproMatrix::MiproMatrix(MinoProgramBank *bank) :
    MinoProgram(bank)
{
    const QColor matrixColor(QColor::fromHslF(0.32, 1.0, 0.5));

    this->setLabel("Matrix");

    MinoAnimationGroup *afg = new MinoAnimationGroup(this);
    MinaFlash *af = new MinaFlash(afg);
    af->setColor(matrixColor);
    afg->addAnimation(af);
    MinaRandomPixels *arp = new MinaRandomPixels(afg);
    arp->setColor(matrixColor);
    arp->setDensity(0.4);
    afg->addAnimation(arp);
    this->addAnimationGroup(afg);

    MinoAnimationGroup *asg = new MinoAnimationGroup(this);
    MinaStars *as = new MinaStars(asg);
    as->setColor(QColor::fromHslF(0.3, 1.0, 0.5));
    as->setDensity(4);
    as->setDuration("2");
    as->setLoopSize("1/2");
    asg->addAnimation(as);
    MinaVibration *av = new MinaVibration(asg);
    av->setColor(QColor::fromHslF(0.32, 1.0, 0.5));
    av->setSegments(0.2);
    asg->addAnimation(av);
    this->addAnimationGroup(asg);

    MinoAnimationGroup *afog = new MinoAnimationGroup(this);
    MinaFallingObjects *afo = new MinaFallingObjects(afog);
    afo->setColor(matrixColor);
    afo->setLoopSize("1/2");
    afo->setDensity(7);
    afo->setDuration(8);
    afo->setLength(0.5);
    afog->addAnimation(afo);
    this->addAnimationGroup(afog);
}
