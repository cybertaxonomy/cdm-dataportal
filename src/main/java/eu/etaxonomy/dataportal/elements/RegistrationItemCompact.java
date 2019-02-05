/**
* Copyright (C) 2019 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal.elements;

import java.util.Objects;

import org.openqa.selenium.NoSuchElementException;
import org.openqa.selenium.WebElement;

public class RegistrationItemCompact extends RegistrationItem{

    /**
     * @param containerElement
     */
    public RegistrationItemCompact(WebElement containerElement) {
        super(containerElement);
        if(style != style.COMPACT){
            throw new NoSuchElementException("RegistrationItem has not the expected style, expected : FULL, but was " + Objects.toString(style, "NULL"));
        }
    }

    public WebElement getSummaryElement() {
        return summaryElement;
    }

}