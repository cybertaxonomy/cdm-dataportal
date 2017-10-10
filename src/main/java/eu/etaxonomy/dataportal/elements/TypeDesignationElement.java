/**
* Copyright (C) 2012 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import org.apache.log4j.Logger;
import org.openqa.selenium.By;
import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.CdmEntityClassAttributes;

/**
 * @author andreas
 * @date Jul 30, 2012
 *
 */
public class TypeDesignationElement extends BaseElement {

    private static final Logger logger = Logger.getLogger(TypeDesignationElement.class);

    private final WebElement status;

    private WebElement nameDescription = null;

    private TypeDesignationType typeDesignationType = null;

    public TypeDesignationElement(WebElement element) {
        super(element);
        status = element.findElement(By.cssSelector(".status"));
        try {
            nameDescription = element.findElement(By.cssSelector(".description"));
        } catch (NoSuchElementException e) {
            // IGNORE
        }
        CdmEntityClassAttributes attr;
        try {
            attr = new CdmEntityClassAttributes(element.getAttribute("class"));
            typeDesignationType = TypeDesignationType.valueOfCdmClass(attr.getCdmType());
        } catch (Exception e) {
            logger.error(e);
        }

    }

    public String statusToString(){
        return status.getText();
    }

    public TypeDesignationType getTypeDesignationType() {
        return typeDesignationType;
    }

    /**
     * contains nomenclatorical status, protologues, etc.
     *
     * @return the nameDescription
     */
    public WebElement getNameDescription() {
        return nameDescription;
    }

}
