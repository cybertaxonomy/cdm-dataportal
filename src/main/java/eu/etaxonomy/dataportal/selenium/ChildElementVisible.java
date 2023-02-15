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
import org.openqa.selenium.WebElement;

import com.google.common.base.Function;

/**
 * @author andreas
 */
public class ChildElementVisible implements Function<WebDriver, Boolean> {

	private By findCondition;
	private WebElement parent;

	public ChildElementVisible(WebElement parent, By by) {
		this.findCondition = by;
		this.parent = parent;
	}

	@Override
    public Boolean apply(WebDriver driver) {
		parent.findElement(this.findCondition);
		return Boolean.valueOf(true);
	}
}