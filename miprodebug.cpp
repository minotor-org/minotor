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

#include "miprodebug.h"

#include "minotor.h"

#include "minadebug.h"
#include "minaflash.h"
#include "minaimage.h"
#include "minaballs.h"
#include "minarainbowoil.h"

#include "minopersistentobjectfactory.h"

MiproDebug::MiproDebug(MinoProgramBank *bank) :
    MinoProgram(bank)
{
    this->setLabel("Debug");

//    for (int i=0;i<35;i++)
//    {
//        MinoAnimationGroup *asgd = new MinoAnimationGroup(this);
//        MinaRainbowOil *ad = new MinaRainbowOil(asgd);
//        ad->setColor(QColor::fromHslF(0.12,0.34,0.56));
//        ad->setLoopSize("2");
//        QList<MidiControllableParameter*> pl = ad->findChildren<MidiControllableParameter*>();
//        for (int n=0;n<3;n++) {
//            if(n<pl.count())
//                pl.at(n)->setPreferred(true);
//        }
//        asgd->addAnimation(ad);
//        this->addAnimationGroup(asgd);
//    }

    QList<MinoAnimationDescription> animations = MinoPersistentObjectFactory::availableAnimationModels();
    foreach(const MinoAnimationDescription& animation, animations)
    {
        MinoAnimationGroup *mag = new MinoAnimationGroup(this);
        mag->addAnimation(animation.className());
        addAnimationGroup(mag);
    }
    /*
    MinoAnimationGroup *asg = new MinoAnimationGroup(this);
    MinaImage *ai = new MinaImage(asg);
    ai->setColor(QColor::fromHslF(0.32, 1.0, 0.5));
    asg->addAnimation(ai);
    this->addAnimationGroup(asg);

    MinoAnimationGroup *asb = new MinoAnimationGroup(this);
    MinaBalls *ab = new MinaBalls(asb);
    ab->setColor(0.32);
    asb->addAnimation(ab);
    this->addAnimationGroup(asb);
*/
}
