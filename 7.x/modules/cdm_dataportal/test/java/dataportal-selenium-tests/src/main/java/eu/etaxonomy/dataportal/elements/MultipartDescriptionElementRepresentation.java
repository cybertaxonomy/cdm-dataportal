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

import java.util.ArrayList;
import java.util.List;

import org.openqa.selenium.WebElement;

public class MultipartDescriptionElementRepresentation extends DescriptionElementRepresentation{

	ArrayList<DescriptionElementRepresentation> multipartElements = null;

	public ArrayList<DescriptionElementRepresentation> getMultipartElements() {
		return multipartElements;
	}

	public MultipartDescriptionElementRepresentation(WebElement ... element) {
		super(element[0]);

		multipartElements = new ArrayList<DescriptionElementRepresentation>(element.length -1);
		for(int i = 0; i < element.length; i++){
			multipartElements.add(new DescriptionElementRepresentation(element[i]));
		}
	}

	@Override
	public WebElement getElement() {
		String methodName = "getElement()";
		throw new RuntimeException(methodName +" is NOT applicable to MultipartDescriptionElementRepresentation, use getElements().get(n)." + methodName + " instead");
	}

	@Override
	public String getText() {
		StringBuilder combinedText = new StringBuilder();
		for (DescriptionElementRepresentation element : multipartElements) {
			combinedText.append(element.getText());
		}
		return combinedText.toString();
	}

	@Override
	public List<String> getClassAttributes() {
		String methodName = "getClassAttributes()";
		throw new RuntimeException(methodName +" is NOT applicable to MultipartDescriptionElementRepresentation, use getElements().get(n)." + methodName + " instead");
	}

	@Override
	public double getComputedFontSize() {
		String methodName = "getComputedFontSize()";
		throw new RuntimeException(methodName +" is NOT applicable to MultipartDescriptionElementRepresentation, use getElements().get(n)." + methodName + " instead");
	}

	@Override
	public List<LinkElement> getLinksInElement() {
		List<LinkElement> combinedLinkList = new ArrayList<LinkElement>();
		for (DescriptionElementRepresentation element : multipartElements) {
			combinedLinkList.addAll(element.getLinksInElement());
		}
		return combinedLinkList;
	}

	@Override
	public String toString() {
		StringBuilder toStringRepresentatin = new StringBuilder(this.getClass().getSimpleName());
		for (DescriptionElementRepresentation element : multipartElements) {
			toStringRepresentatin.append("<" + element.getElement().getTagName() + ">");
		}
		return toStringRepresentatin.toString();
	}

	@Override
	public List getSources() {
		List<BaseElement> combinedSourcesList = new ArrayList<BaseElement>();
		for (DescriptionElementRepresentation element : multipartElements) {
			combinedSourcesList.addAll(element.getSources());
		}
		return combinedSourcesList;
	}


}