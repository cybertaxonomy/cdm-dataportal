// $Id$
/**
* Copyright (C) 2011 EDIT
* European Distributed Institute of Taxonomy
* http://www.e-taxonomy.eu
*
* The contents of this file are subject to the Mozilla Public License Version 1.1
* See LICENSE.TXT at the top of this package for the full license terms.
*/
package eu.etaxonomy.dataportal;

import java.util.ArrayList;
import java.util.List;

import org.openqa.selenium.By;
import org.openqa.selenium.WebElement;

import eu.etaxonomy.dataportal.elements.BaseElement;
import eu.etaxonomy.dataportal.elements.LinkElement;

/**
 * @author andreas
 * @date Sep 16, 2011
 *
 */
public class ElementUtils {

	/**
	 * @param fnListElements
	 * @return
	 */
	public static List<BaseElement> baseElementsFromFootNoteListElements(List<WebElement> fnListElements) {
		List<BaseElement> footNotes = new ArrayList<BaseElement>();
		for(WebElement fn : fnListElements) {
			footNotes.add(new BaseElement(fn));
		}
		return footNotes;
	}

	/**
	 * @param fnkListElements
	 * @return
	 */
	public static List<LinkElement> linkElementsFromFootNoteKeyListElements(List<WebElement> fnkListElements) {
		List<LinkElement> footNoteKeys = new ArrayList<LinkElement>();
		for(WebElement fnk : fnkListElements) {
			footNoteKeys.add(new LinkElement(fnk));
		}
		return footNoteKeys;
	}

}
