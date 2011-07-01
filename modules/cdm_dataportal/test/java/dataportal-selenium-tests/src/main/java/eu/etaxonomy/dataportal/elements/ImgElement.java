package eu.etaxonomy.dataportal.elements;

import java.awt.Dimension;
import java.net.MalformedURLException;
import java.net.URL;

import org.openqa.selenium.RenderedWebElement;

public class ImgElement extends BaseElement {

	private URL url = null;
	private Dimension dimension = null;
	private String altText = null;

	public ImgElement(RenderedWebElement img) {

		super(img);

		// read src url
		if (img.getAttribute("src") != null) {
			try {
				setUrl(new URL(img.getAttribute("src")));
			} catch (MalformedURLException e) {
				// IGNORE //
			}
		}

		// read rendered width & height
		setDimension(img.getSize());

		setAltText(img.getAttribute("alt"));
	}

	public void setUrl(URL url) {
		this.url = url;
	}

	public URL getUrl() {
		return url;
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