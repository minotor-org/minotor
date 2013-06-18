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

#include "miprowaves.h"

#include "minotor.h"

#include "minawaveform.h"

MiproWaves::MiproWaves(MinoProgramBank *bank) :
    MinoProgram(bank)
{
    this->setLabel("Waves");
    MinoAnimationGroup *awfg = new MinoAnimationGroup(this);
    MinaWaveform *awf = new MinaWaveform(awfg);
    awf->setColor(QColor::fromHslF(0.1, 1.0, 0.5));
    awfg->addAnimation(awf);
    this->addAnimationGroup(awfg);

    MinoAnimationGroup *awf1g = new MinoAnimationGroup(this);
    MinaWaveform *awf1 = new MinaWaveform(awf1g);
    awf1->setColor(QColor::fromHslF(0.3, 1.0, 0.5));
    awf1g->addAnimation(awf1);
    this->addAnimationGroup(awf1g);

    MinoAnimationGroup *awf2g = new MinoAnimationGroup(this);
    MinaWaveform *awf2 = new MinaWaveform(awf2g);
    awf2->setColor(QColor::fromHslF(0.6, 1.0, 0.5));
    awf2g->addAnimation(awf2);
    this->addAnimationGroup(awf2g);

    MinoAnimationGroup *awf3g = new MinoAnimationGroup(this);
    MinaWaveform *awf3 = new MinaWaveform(awf3g);
    awf3->setColor(QColor::fromHslF(0.9, 1.0, 0.5));
    awf3g->addAnimation(awf3);
    this->addAnimationGroup(awf3g);
}
