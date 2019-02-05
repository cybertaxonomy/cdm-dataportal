/**
* Copyright (C) 2019 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import java.util.List;
import java.util.Objects;

import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebElement;

public class RegistrationItemFull extends RegistrationItem{

    /**
     * @param containerElement
     */
    public RegistrationItemFull(WebElement containerElement) {
        super(containerElement);
        if(style != style.FULL){
            throw new NoSuchElementException("RegistrationItem has not the expected style, expected : FULL, but was " + Objects.toString(style, "NULL"));
        }
    }

    public WebElement getNameElement() {
        return nameElement;
    }

    public List<WebElement> getSpecimenTypeDesignations() {
        return specimenTypeDesignations;
    }

    public List<WebElement> getNameTypeDesignations() {
        return nameTypeDesignations;
    }

    public WebElement getCitation() {
        return citation;
    }
}