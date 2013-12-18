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

    private final ClassificationTreeElement focusedElement;

    private final RemoteWebElement viewPortElement;

    public ClassificationTreeElement getFocusedElement(){
        return focusedElement;
    }

    public ClassificationTreeBlock(WebElement element) {
        super(element);

        focusedElement = new ClassificationTreeElement(element.findElement(By.className("focused")));
        viewPortElement = (RemoteWebElement)element.findElement(By.className("cdm_taxontree_scroller_xy"));
    }

    public boolean isVisibleInViewPort(ClassificationTreeElement element){
        Point elementOffset = ((RemoteWebElement)element.getElement()).getCoordinates().inViewPort();
        Point viewPortElementOffset = viewPortElement.getCoordinates().inViewPort();
        Point elementRelativeToViewPort = elementOffset.moveBy(-1 * viewPortElementOffset.x, -1 * viewPortElementOffset.y);
        Dimension viewPortDimension = viewPortElement.getSize();
        return elementRelativeToViewPort.y < viewPortDimension.height;
    }


}
