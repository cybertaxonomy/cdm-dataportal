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
import org.openqa.selenium.WebElement;

/**
 * @author andreas
 * @date Jul 30, 2012
 *
 */
public class TypeDesignationElement extends BaseElement {

    private WebElement status;

    private TypeDesignationType typeDesignationType;

    public TypeDesignationElement(WebElement element) {
        super(element);
        status = element.findElement(By.cssSelector(".status"));
        typeDesignationType = TypeDesignationType.valueOf(element.getAttribute("class"));
    }

    public String statusToString(){
        return status.getText();
    }

    public TypeDesignationType getTypeDesignationType() {
        return typeDesignationType;
    }

}
