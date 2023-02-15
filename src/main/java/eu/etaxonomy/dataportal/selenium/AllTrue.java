/**
 * Copyright (C) 2009 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */
package eu.etaxonomy.dataportal.selenium;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.openqa.selenium.WebDriver;

import com.google.common.base.Function;

/**
 * @author andreas
 */
public class AllTrue implements Function<WebDriver, Boolean> {

    List<Function<WebDriver, Boolean>> functions = new ArrayList<>();

    @SafeVarargs
    public AllTrue(Function<WebDriver, Boolean> ... functions) {
        if(functions == null){
            throw new NullPointerException("Constructor parameter mus not be null");
        }
        this.functions = Arrays.asList(functions);
    }

    @Override
    public Boolean apply(WebDriver driver) {
        Boolean allTrue = true;
        for(Function<WebDriver, Boolean> f : functions){
            allTrue &= f.apply(driver);
        }
        return allTrue;
    }
}