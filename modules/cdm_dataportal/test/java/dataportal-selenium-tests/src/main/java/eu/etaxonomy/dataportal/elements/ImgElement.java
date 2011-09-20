package eu.etaxonomy.dataportal.elements;

import java.net.MalformedURLException;
import java.net.URL;

import org.openqa.selenium.Dimension;
import org.openqa.selenium.WebElement;

public class ImgElement extends BaseElement {

	private URL srcUrl = null;
	private Dimension dimension = null;
	private String altText = null;

	public ImgElement(WebElement img) {

		super(img);

		// read src url
		if (img.getAttribute("src") != null) {
			try {
				setSrcUrl(new URL(img.getAttribute("src")));
			} catch (MalformedURLException e) {
				// IGNORE //
			}
		}

		// read rendered width & height
		setDimension(img.getSize());

		setAltText(img.getAttribute("alt"));
	}

	public void setSrcUrl(URL url) {
		this.srcUrl = url;
	}

	/**
	 * Returns the image source URL from the src attribute
	 * @return
	 */
	public URL getSrcUrl() {
		return srcUrl;
	}

	public void setDimension(Dimension dimension) {
		this.dimension = dimension;
	}

	public Dimension getDimension() {
		return dimension;
	}

	public void setAltText(String altText) {
		this.altText = altText;
	}

	public String getAltText() {
		return altText;
	}


}