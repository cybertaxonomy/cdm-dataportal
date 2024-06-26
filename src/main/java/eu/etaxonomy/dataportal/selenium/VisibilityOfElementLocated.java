/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium;

import org.openqa.selenium.By;
import org.openqa.selenium.WebDriver;

import com.google.common.base.Function;

/**
 * @author andreas
 */
public class VisibilityOfElementLocated implements Function<WebDriver, Boolean> {

    private By findCondition;

    public VisibilityOfElementLocated(By by) {
        this.findCondition = by;
    }

    @Override
    public Boolean apply(WebDriver driver) {
        driver.findElement(this.findCondition);
        return Boolean.valueOf(true);
    }
}