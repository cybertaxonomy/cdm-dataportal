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

import com.google.common.base.Function;

/**
 * @author andreas
 */
public class UrlLoaded implements Function<WebDriver, Boolean> {

	private String url;

	public UrlLoaded(String url) {
		this.url = url;
	}

	@Override
    public Boolean apply(WebDriver driver) {
		return driver.getCurrentUrl().equals(url);
	}
}