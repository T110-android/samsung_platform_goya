package ch.ethz.vppserver.ippclient;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlPullParserFactory;

import ch.ethz.vppserver.schema.ippclient.AttributeGroup;
import ch.ethz.vppserver.schema.ippclient.AttributeList;
import ch.ethz.vppserver.schema.ippclient.AttributeValue;
import ch.ethz.vppserver.schema.ippclient.Keyword;
import ch.ethz.vppserver.schema.ippclient.SetOfEnum;
import ch.ethz.vppserver.schema.ippclient.SetOfKeyword;
import ch.ethz.vppserver.schema.ippclient.Tag;
import ch.ethz.vppserver.schema.ippclient.TagList;
import ch.ethz.vppserver.schema.ippclient.Attribute;
import ch.ethz.vppserver.schema.ippclient.Enum;

/**
 * Copyright (C) 2008 ITS of ETH Zurich, Switzerland, Sarah Windler Burri
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 3 of the License, or (at your option) any
 * later version.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.
 * 
 * See the GNU Lesser General Public License for more details. You should have
 * received a copy of the GNU Lesser General Public License along with this
 * program; if not, see <http://www.gnu.org/licenses/>.
 */
public class IppJaxb {
  private List<Tag> _tagList = null;
  private List<AttributeGroup> _attributeGroupList = null;

  IppJaxb() throws FileNotFoundException, XmlPullParserException {
	  XmlPullParserFactory factory = XmlPullParserFactory.newInstance();
	  factory.setNamespaceAware(true);
	  XmlPullParser xpp = factory.newPullParser();
	  InputStream inputStream = IppJaxb.class.getResourceAsStream("ipplistofattributes.xml");
	  xpp.setInput(inputStream, "UTF8");
	  
	  AttributeList attributeList = parseAttributeList(xpp);
	  _attributeGroupList = attributeList.getAttributeGroup();
	  
	  inputStream = IppJaxb.class.getResourceAsStream("ipplistoftag.xml");
	  xpp.setInput(inputStream, "UTF8");
	  TagList tagList = parseTagList(xpp);
	 _tagList = tagList.getTag();
  }


  /**
   * 
   * @return
   */
  public List<Tag> getTagList() {

    return _tagList;
  }

  /**
   * 
   * @return
   */
  public List<AttributeGroup> getAttributeGroupList() {
    return _attributeGroupList;
  }
 
  private TagList parseTagList(XmlPullParser parser) {
	  try { 
		  TagList tagList = new TagList();
		  int eventType = parser.getEventType();
		  while(eventType != XmlPullParser.END_DOCUMENT) {
			  if (eventType == XmlPullParser.START_TAG && parser.getName().equals("tag")) {
				  Tag tag = new Tag();
				  tag.setName(parser.getAttributeValue(null, "name"));
				  tag.setValue(parser.getAttributeValue(null, "value"));
				  tag.setDescription(parser.getAttributeValue(null, "description"));
				  String max = parser.getAttributeValue(null, "max");
				  if(max != null) {
					  tag.setMax(Short.parseShort(max));
				  }
				  tagList.getTag().add(tag);
			  }
			  eventType = parser.next();
		  }
		  return tagList;
		} catch (XmlPullParserException e) {
			return null;
		} catch (IOException e) {
			return null;
		}
  }
  
  private AttributeList parseAttributeList(XmlPullParser parser) {
	  try {
		AttributeList attributeList = new AttributeList();
		int eventType = parser.getEventType();
		while(eventType != XmlPullParser.END_DOCUMENT) {
			if (eventType == XmlPullParser.START_TAG && parser.getName().equals("attribute-group")) {
			  AttributeGroup attributeGroup = new AttributeGroup();
			  attributeGroup.setTag(parser.getAttributeValue(null, "tag"));
			  attributeGroup.setTagName(parser.getAttributeValue(null, "tag-name"));
			  attributeGroup.setDescription(parser.getAttributeValue(null, "description"));
			  if(!parser.isEmptyElementTag()) {
				  eventType = parser.next();
				  while(!(eventType == XmlPullParser.END_TAG && parser.getName().equals("attribute-group"))) {
					  if (eventType == XmlPullParser.START_TAG && parser.getName().equals("attribute")) {
						  Attribute attribute = new Attribute();
						  attribute.setName(parser.getAttributeValue(null, "name"));
						  attribute.setDescription(parser.getAttributeValue(null, "description"));
						  if(!parser.isEmptyElementTag()) {
							  eventType = parser.next();
							  while(!(eventType == XmlPullParser.END_TAG && parser.getName().equals("attribute"))) {
								  if (eventType == XmlPullParser.START_TAG && parser.getName().equals("attribute-value")) {
									  AttributeValue attributeValue = new AttributeValue();
									  attributeValue.setTag(parser.getAttributeValue(null, "tag"));
									  attributeValue.setTagName(parser.getAttributeValue(null, "tag-name"));
									  attributeValue.setValue(parser.getAttributeValue(null, "value"));
									  attributeValue.setDescription(parser.getAttributeValue(null, "description"));
									  if(!parser.isEmptyElementTag()) {
										  eventType = parser.next();
										  while(!(eventType == XmlPullParser.END_TAG && parser.getName().equals("attribute-value"))) {
											  if (eventType == XmlPullParser.START_TAG && parser.getName().equals("set-of-enum")) {
												  SetOfEnum setOfEnum = new SetOfEnum();
												  if(!parser.isEmptyElementTag()) {
													  eventType = parser.next();
													  while(!(eventType == XmlPullParser.END_TAG && parser.getName().equals("set-of-enum"))) {
														  if (eventType == XmlPullParser.START_TAG && parser.getName().equals("enum")) {
															  Enum e = new Enum();
															  e.setName(parser.getAttributeValue(null, "name"));
															  e.setValue(parser.getAttributeValue(null, "value"));
															  e.setDescription(parser.getAttributeValue(null, "description"));
															  setOfEnum.getEnum().add(e);
														  }
														  eventType = parser.next();
													  }
												  }
												  attributeValue.setSetOfEnum(setOfEnum);
											  } else if (eventType == XmlPullParser.START_TAG && parser.getName().equals("set-of-keyword")) {
												  SetOfKeyword setOfKeyword = new SetOfKeyword();
												  if(!parser.isEmptyElementTag()) {
													  eventType = parser.next();
													  while(!(eventType == XmlPullParser.END_TAG && parser.getName().equals("set-of-keyword"))) {
														  if (eventType == XmlPullParser.START_TAG && parser.getName().equals("keyword")) {
															  Keyword e = new Keyword();
															  e.setValue(parser.getAttributeValue(null, "value"));
															  e.setDescription(parser.getAttributeValue(null, "description"));
															  setOfKeyword.getKeyword().add(e);
														  }
														  eventType = parser.next();
													  }
												  }
												  attributeValue.setSetOfKeyword(setOfKeyword);
											  }
											  eventType = parser.next();
										  }									  
									  }
									  attribute.getAttributeValue().add(attributeValue);
								  }
								  eventType = parser.next();
							  }
						  }
						  attributeGroup.getAttribute().add(attribute);
					  }
					  eventType = parser.next();
				  } 
				attributeList.getAttributeGroup().add(attributeGroup);
			  }
			}
			eventType = parser.next();
		}
		return attributeList;
	} catch (XmlPullParserException e) {
		return null;
	} catch (IOException e) {
		return null;
	}
  }  
}
