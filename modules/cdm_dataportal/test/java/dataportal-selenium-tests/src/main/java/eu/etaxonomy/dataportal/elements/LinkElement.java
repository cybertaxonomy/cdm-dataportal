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

import java.net.MalformedURLException;
import java.net.URL;
import java.util.Arrays;
import java.util.List;

import org.openqa.selenium.RenderedWebElement;

/**
 * @author andreas
 * @date Jul 1, 2011
 *
 */
public class LinkElement extends BaseElement {

	private URL url = null;
	public URL getUrl() {
		return url;
	}

	public void setUrl(URL url) {
		this.url = url;
	}


	public LinkElement(RenderedWebElement a) {

		super(a);

		// read src url
		if (a.getAttribute("href") != null) {
			try {
				setUrl(new URL(a.getAttribute("href")));
			} catch (MalformedURLException e) {
				// IGNORE //
			}
		}

	}

}
