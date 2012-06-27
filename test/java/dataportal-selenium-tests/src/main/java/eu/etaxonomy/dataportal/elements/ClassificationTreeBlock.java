// $Id$
/**
* Copyright (C) 2012 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import org.openqa.selenium.By;
import org.openqa.selenium.Dimension;
import org.openqa.selenium.Point;
import org.openqa.selenium.WebElement;
import org.openqa.selenium.remote.RemoteWebElement;

import com.sun.corba.se.impl.interceptors.PINoOpHandlerImpl;

/**
 *
 * <div class="cdm_taxontree_scroller_xy">
 * <ul class="cdm_taxontree"><li
 *
 * @author andreas
 * @date May 21, 2012
 *
 */
public class ClassificationTreeBlock extends DrupalBlock {

    private ClassificationTreeElement focusedElement;

    private RemoteWebElement viewPortElement;

    public ClassificationTreeElement getFocusedElement(){
        return focusedElement;
    }

    public ClassificationTreeBlock(WebElement element) {
        super(element);

        focusedElement = new ClassificationTreeElement(element.findElement(By.className("focused")));
        viewPortElement = (RemoteWebElement)element.findElement(By.className("cdm_taxontree_scroller_xy"));
    }

    public Point positionInViewPort(ClassificationTreeElement element){
        Point elementLocation = ((RemoteWebElement)element.getElement()).getLocationOnScreenOnceScrolledIntoView();
        Point viewPortLocation = viewPortElement.getLocationOnScreenOnceScrolledIntoView();
        return new Point(elementLocation.x - viewPortLocation.x, elementLocation.y - viewPortLocation.y);
    }

    public boolean isVisibleInViewPort(ClassificationTreeElement element){
        Point elementOffset = positionInViewPort(element);
        Dimension viewPortDimension = viewPortElement.getSize();
        return elementOffset.y < viewPortDimension.height;
    }


}
