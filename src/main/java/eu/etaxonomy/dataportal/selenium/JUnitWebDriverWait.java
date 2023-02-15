/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium;

import org.openqa.selenium.WebDriver;
import org.openqa.selenium.support.ui.WebDriverWait;

/**
 * @author andreas
 */
public class JUnitWebDriverWait extends WebDriverWait {

    public JUnitWebDriverWait(WebDriver driver, long timeOutInSeconds) {
        super(driver, timeOutInSeconds);
    }

    @Override
    protected RuntimeException timeoutException(String message, Throwable lastException) {
        throw new AssertionError(message);
     }
}