<xsl:stylesheet version="1.0" xmlns:ns0="http://www.w3.org/1999/xhtml" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:xlink="http://www.w3.org/1999/xlink" xmlns:fb="http://www.gribuser.ru/xml/fictionbook/2.0" exclude-result-prefixes="xlink fb">
    <xsl:output method="xml" encoding="utf-8" omit-xml-declaration="yes" />
    <xsl:strip-space elements="*" />
    <xsl:preserve-space elements="style" />
    <xsl:key name="note-link" match="fb:section" use="@id" />
    <xsl:template match="/fb:FictionBook">
        <html lang="ru">
            <xsl:text></xsl:text>
            <head>
                <xsl:text></xsl:text>
                <meta charset="utf-8" />
                <xsl:text></xsl:text>
                <title>
                    <xsl:value-of select="fb:description/fb:title-info/fb:author/fb:first-name" />
                    <xsl:text></xsl:text>
                    <xsl:value-of select="fb:description/fb:title-info/fb:author/fb:last-name" />.
                    <xsl:value-of select="fb:description/fb:title-info/fb:book-title" /></title>
                <xsl:text></xsl:text>
                <link rel="stylesheet" href="/conf/book.css" />
                <xsl:text></xsl:text>
            </head>
            <xsl:text></xsl:text>
            <body>
                <xsl:text></xsl:text>
                <xsl:for-each select="fb:description/fb:title-info/fb:coverpage/fb:image">
                    <xsl:call-template name="image" />
                </xsl:for-each>
                <xsl:apply-templates select="fb:body" />
            </body>
            <xsl:text></xsl:text>
        </html>
        <xsl:text></xsl:text>
    </xsl:template>




    <xsl:template match="fb:section">
        <xsl:text></xsl:text>
        <xsl:variable name="sectionId" select="generate-id(.)" />

        <!--<xsl:text>SECTION_</xsl:text>-->
       <!-- <xsl:copy-of select="$sectionId" />-->

<!-- SECTION-->
        <!-- !!! sections not supported in qt  -->
        <!--        <xsl:choose>
                <xsl:when test="@id">
                    <p>
                        <xsl:attribute name="id">
                            <xsl:text>SECTION_ID_</xsl:text>
                            <xsl:copy-of select="@id"/>
                        </xsl:attribute>
                    </p>
                </xsl:when>
                <xsl:otherwise>-->



<p>
    <xsl:attribute name="id">

        <xsl:text>SECTION_</xsl:text>
        <xsl:copy-of select="$sectionId" />

    </xsl:attribute>
</p>




<!--
        <p>
            <xsl:attribute name="id">

                <xsl:copy-of select="@id"/>
                <xsl:text>SECTION_</xsl:text>
                <xsl:copy-of select="$sectionId" />

            </xsl:attribute>
        </p>
        -->



        <!--                </xsl:otherwise>
            </xsl:choose>-->
        <xsl:apply-templates />
        <xsl:text></xsl:text>
    </xsl:template>

    <xsl:template match="fb:body[@name='notes']/fb:section">
        <xsl:text></xsl:text>
<!--        <xsl:text>NOTES_SECTION_</xsl:text>-->
<!--        <xsl:copy-of select="@id" />-->
<!--        <xsl:value-of select="@id" />-->
        <p>
            <xsl:attribute name="id">
                <xsl:text>NOTES_SECTION_</xsl:text>
                <xsl:copy-of select="@id" />
            </xsl:attribute>
        </p>
        <xsl:apply-templates />
<!--        <br />-->
        <xsl:text></xsl:text>
    </xsl:template>


<xsl:template match="fb:body[@name='footnotes']/fb:section">
    <xsl:text></xsl:text>
<!--        <xsl:text>NOTES_SECTION_</xsl:text>-->
<!--        <xsl:copy-of select="@id" />-->
<!--        <xsl:value-of select="@id" />-->
    <p>
    <xsl:attribute name="id">
        <xsl:copy-of select="@id" />
    </xsl:attribute>
    </p>
    <xsl:apply-templates />
<!--        <br />-->
    <xsl:text></xsl:text>
</xsl:template>





    <xsl:template match="fb:body/fb:title/fb:p">
        <xsl:apply-templates />
        <xsl:if test="position() != last()">
            <br />
            <br />
            <xsl:text></xsl:text>
        </xsl:if>
    </xsl:template>
    <xsl:template match="fb:body[@name='footnotes']">
        <xsl:text></xsl:text>
        <hr />
        <xsl:text></xsl:text>

<p>
    <xsl:attribute name="id">
       <!-- <xsl:text>NOTES_SECTION_</xsl:text>-->
        <xsl:copy-of select="@id" />
    </xsl:attribute>
</p>
<xsl:apply-templates />
        <xsl:text></xsl:text>
    </xsl:template>

    <xsl:template match="fb:title">
        <xsl:text></xsl:text>
        <xsl:choose>
            <xsl:when test="count(ancestor::node()) &lt; 9">
                <xsl:element name="{concat('h',count(ancestor::node())-2)}">
                    <xsl:apply-templates />
                </xsl:element>
            </xsl:when>
            <xsl:otherwise>
                <xsl:element name="h6">
                    <xsl:apply-templates />
                </xsl:element>
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    <xsl:template match="fb:section/fb:title/fb:p">
        <xsl:apply-templates />
        <xsl:if test="position() != last()">
            <br />
<!--            <xsl:text>&#160;</xsl:text>-->
        </xsl:if>
    </xsl:template>
    <xsl:template match="fb:subtitle">
        <h5>
            <xsl:apply-templates />
        </h5>
    </xsl:template>
    <xsl:template match="fb:p">
        <xsl:text></xsl:text>
        <p>
            <xsl:copy-of select="@id" />
            <xsl:apply-templates />
        </p>
    </xsl:template>
    <xsl:template match="fb:strong">
        <strong>
            <xsl:apply-templates />
        </strong>
    </xsl:template>
    <xsl:template match="fb:emphasis">
        <em>
            <xsl:apply-templates />
        </em>
    </xsl:template>
    <xsl:template match="fb:sup">
        <sup>
            <xsl:apply-templates />
        </sup>
    </xsl:template>
    <xsl:template match="fb:sub">
        <sub>
            <xsl:apply-templates />
        </sub>
    </xsl:template>
    <xsl:template match="fb:style">
        <span class="{@name}">
            <xsl:apply-templates />
        </span>
    </xsl:template>
    <xsl:template match="fb:empty-line">
        <xsl:text></xsl:text>
        <!-- <p>&#160;</p>-->
    </xsl:template>
    <xsl:template match="fb:a">
        <xsl:element name="a">
            <xsl:attribute name="href">
                <xsl:value-of select="@xlink:href" />
            </xsl:attribute>
            <xsl:choose>
                <xsl:when test="(@type) = 'note'">
                    <sup>
                        <xsl:apply-templates />
                    </sup>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:apply-templates />
                </xsl:otherwise>
            </xsl:choose>
        </xsl:element>
    </xsl:template>
    <xsl:template name="annotation">
        <h3>Annotation</h3>
        <xsl:apply-templates />
    </xsl:template>
    <xsl:template match="fb:epigraph">
        <blockquote class="epigraph">
            <xsl:apply-templates />
        </blockquote>
    </xsl:template>
    <xsl:template match="fb:text-author">
        <blockquote>
            <em>
                <xsl:apply-templates />
            </em>
        </blockquote>
    </xsl:template>
    <xsl:template match="fb:cite">
        <xsl:text></xsl:text>
        <blockquote>
            <xsl:apply-templates />
            <xsl:text></xsl:text>
        </blockquote>
    </xsl:template>
    <xsl:template match="fb:date">
        <xsl:choose>
            <xsl:when test="not(@value)">
                <xsl:apply-templates />
                <br />
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="@value" />
                <br />
            </xsl:otherwise>
        </xsl:choose>
    </xsl:template>
    <xsl:template match="fb:poem">
        <xsl:text></xsl:text>
        <blockquote class="poem">
            <xsl:apply-templates />
        </blockquote>
    </xsl:template>
    <xsl:template match="fb:stanza">
        <xsl:text></xsl:text>
        <section class="stanza">
            <xsl:apply-templates />
            <xsl:text></xsl:text>
        </section>
        <xsl:text></xsl:text>
    </xsl:template>
    <xsl:template match="fb:v">
        <xsl:text></xsl:text>
        <p>
            <xsl:apply-templates />
        </p>
    </xsl:template>
    <xsl:template match="fb:image" name="image">
        <xsl:text></xsl:text>
        <div>
            <xsl:copy-of select="@id" />
            <img>
                <!--!!!-->
                <!-- <xsl:attribute name="height" >
                    <xsl:text>100%</xsl:text>
                </xsl:attribute>-->
                <xsl:choose>
                    <xsl:when test="starts-with(@xlink:href,'#')">
                        <!-- !!! Very important piece of code. Othervise Qt doesn't want
                        to display $href. But if the $href is used somewhere then it's ok
                        and can be used to access binary by id -->
                        <xsl:variable name="href" select="substring-after(@xlink:href,'#')" />
                        <xsl:attribute name="id">
                            <xsl:value-of select="$href" />
                        </xsl:attribute>
                        <xsl:attribute name="src">
                            <xsl:text>data:</xsl:text>
                            <xsl:value-of select="//fb:binary[@id=$href]/@content-type" />
                            <xsl:text>;base64,</xsl:text>
                            <xsl:value-of select="//fb:binary[@id=$href]" />
                        </xsl:attribute>
                    </xsl:when>
                    <xsl:otherwise>
                        <xsl:attribute name="src">
                            <xsl:value-of select="@xlink:href" />
                        </xsl:attribute>
                    </xsl:otherwise>
                </xsl:choose>
            </img>
        </div>
    </xsl:template>
    <xsl:template match="fb:table">
        <xsl:text></xsl:text>
        <table>
            <xsl:text></xsl:text>
            <tbody>
                <xsl:apply-templates />
                <xsl:text></xsl:text>
            </tbody>
            <xsl:text></xsl:text>
        </table>
    </xsl:template>
    <xsl:template match="fb:tr">
        <xsl:text></xsl:text>
        <tr>
            <xsl:copy-of select="@*|b/@*" />
            <xsl:apply-templates />
        </tr>
    </xsl:template>
    <xsl:template match="fb:td">
        <td>
            <xsl:copy-of select="@*|b/@*" />
            <xsl:apply-templates />
        </td>
    </xsl:template>
    <xsl:template match="fb:th">
        <th>
            <xsl:copy-of select="@*|b/@*" />
            <xsl:apply-templates />
        </th>
    </xsl:template>
</xsl:stylesheet>
