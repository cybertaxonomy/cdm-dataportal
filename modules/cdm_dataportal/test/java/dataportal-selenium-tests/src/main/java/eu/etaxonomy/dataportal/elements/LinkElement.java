// $Id$
/**
 * Copyright (C) 2011 EDIT
 * European Distributed Institute of Taxonomy
 * http://www.e-taxonomy.eu
 *
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * See LICENSE.TXT at the top of this package for the full license terms.
 */

package eu.etaxonomy.dataportal.elements;

import org.openqa.selenium.WebElement;

/**
 * @author andreas
 * @date Jul 1, 2011
 *
 */
public class LinkElement extends BaseElement {

	private String href = null;
	public String getUrl() {
		return href;
	}

	public void setHref(String href) {
		this.href = href;
	}


	public LinkElement(WebElement a) {

		super(a);

		// read src url
		if (a.getAttribute("href") != null) {
				setHref(a.getAttribute("href"));
		}

	}

}
