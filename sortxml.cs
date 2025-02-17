/*!
	Copyright (c) 2014-2020 Kody Brown (@wasatchwizard)

	MIT License:

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to
	deal in the Software without restriction, including without limitation the
	rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
	sell copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
	DEALINGS IN THE SOFTWARE.
*/

using System;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Text;
using System.Xml;

namespace sortxml
{
    class Program
    {
        static bool sort_node = true;
        static bool sort_attr = true;
        static bool pretty = true;
        static bool pause = false;
        static bool overwriteSelf = false;
        static StringComparison sort_node_comp = StringComparison.CurrentCulture; // Default to case-sensitive sorting.
        static StringComparison sort_attr_comp = StringComparison.CurrentCulture;

        static string primary_attr = "";

        static int Main( string[] arguments )
        {
            var inf = "";
            var outf = "";

            var doc = new XmlDocument();

            for (var i = 0; i < arguments.Length; i++) {
                var a = arguments[i];

                if (a[0] == '-' || a[0] == '/' || a[0] == '!') {
                    while (a[0] == '-' || a[0] == '/') {
                        a = a.Substring(1);
                    }
                    var al = a.ToLower();

                    if (al.Equals("?") || al.Equals("help")) {
                        usage();
                        return 0;

                    } else if (al.Equals("p") || al.Equals("pause")) {
                        pause = true;
                    } else if (al.Equals("i") || al.StartsWith("casei") || al.StartsWith("case-i")) {
                        sort_node_comp = StringComparison.CurrentCultureIgnoreCase;
                        sort_attr_comp = StringComparison.CurrentCultureIgnoreCase;
                    } else if (al.Equals("!i") || al.StartsWith("cases") || al.StartsWith("case-s")) {
                        sort_node_comp = StringComparison.CurrentCulture;
                        sort_attr_comp = StringComparison.CurrentCulture;
                    } else if (al.Equals("s") || al.Equals("sort") || al.StartsWith("sortall") || al.StartsWith("sort-all")) {
                        sort_node = true;
                        sort_attr = true;
                    } else if (al.Equals("!s") || al.Equals("!sort") || al.StartsWith("!sortall") || al.StartsWith("!sort-all")) {
                        sort_node = false;
                        sort_attr = false;
                    } else if (al.StartsWith("sortn") || al.StartsWith("sort-n")) {
                        sort_node = true;
                    } else if (al.StartsWith("!sortn") || al.StartsWith("!sort-n")) {
                        sort_node = false;
                    } else if (al.StartsWith("sorta") || al.StartsWith("sort-a")) {
                        sort_attr = true;
                    } else if (al.StartsWith("!sorta") || al.StartsWith("!sort-a")) {
                        sort_attr = false;
                    } else if (al.StartsWith("pretty") || al.StartsWith("!pretty")) {
                        pretty = al.StartsWith("pretty");
                    } else if (al.StartsWith("overwrite") || al.StartsWith("!overwrite")) {
                        overwriteSelf = al.StartsWith("overwrite");
                    } else if (al.StartsWith("primary:")) {
                        primary_attr = al.Substring("primary:".Length);
                    }
                } else {
                    if (inf.Length == 0) {
                        inf = a;
                    } else if (outf.Length == 0) {
                        outf = a;
                    } else {
                        Console.WriteLine("**** Unknown command: " + a);
                    }
                }
            }

            if (inf.Length == 0) {
                usage();
                return 1;
            }

            try {
                doc.LoadXml(File.ReadAllText(inf));
                doc.PreserveWhitespace = !pretty;
            } catch (Exception ex) {
                Console.WriteLine("**** Could not load input file");
                Console.WriteLine(ex.Message);
                return 100;
            }

            if (sort_attr) {
                if (string.IsNullOrEmpty(primary_attr)) {
                    primary_attr = "GUID";
                }
                SortNodeAttrs(doc.DocumentElement);
            }
            if (sort_node) {
                SortNodes(doc.DocumentElement);
            }

            if (outf.Length == 0 && overwriteSelf) {
                outf = inf;
            }

            if (outf.Length > 0) {
                try {
                    doc.Save(outf);
                } catch (Exception ex) {
                    Console.WriteLine("**** Could not save output file");
                    Console.WriteLine(ex.Message);
                    return 101;
                }
            } else {
                doc.Save(Console.Out);
            }

            if (pause) {
                Console.Write("Press any key to quit: ");
                Console.ReadKey(true);
                Console.WriteLine();
            }

            return 0;
        }

        static void SortNodes( XmlNode node )
        {
            // Go down to the furthest child and start there..
            // That is so I can include child nodes in the current node's sort,
            // if all of it's attributes match..
            for (int i = 0, len = node.ChildNodes.Count; i < len; i++) {
                SortNodes(node.ChildNodes[i]);
            }

            // Remove, sort, then re-add the node's children.
            if (sort_node && node.ChildNodes != null && node.ChildNodes.Count > 0) {
                var nodes = new List<XmlNode>(node.ChildNodes.Count);

                for (var i = node.ChildNodes.Count - 1; i >= 0; i--) {
                    nodes.Add(node.ChildNodes[i]);
                    node.RemoveChild(node.ChildNodes[i]);
                }

                nodes.Sort(SortDelegate);

                for (var i = 0; i < nodes.Count; i++) {
                    node.AppendChild(nodes[i]);
                }
            }
        }

        static int SortDelegate( XmlNode a, XmlNode b )
        {
            var result = string.Compare(a.Name, b.Name, sort_node_comp);

            // NOTE: Always sort the _nodes_ based on its attributes (when the
            //       name matches), but don't actually sort the node's attributes.
            //       Sorting attributes, if specified, is done before node sorting happens..

            if (result == 0) {
                var col1 = (a.Attributes.Count >= b.Attributes.Count) ? a.Attributes : b.Attributes;
                var col2 = (a.Attributes.Count >= b.Attributes.Count) ? b.Attributes : a.Attributes;

                for (var i = 0; i < col1.Count; i++) {
                    if (i < col2.Count) {
                        var aa = col1[i];
                        var bb = col2[i];
                        result = string.Compare(aa.Name, bb.Name, sort_attr_comp);
                        if (result == 0) {
                            result = string.Compare(aa.Value, bb.Value, sort_attr_comp);
                            if (result != 0) {
                                return result;
                            }
                            // Attribute name and value match.. continue loop.
                        } else {
                            return result;
                        }
                    } else {
                        return 1;
                    }
                }

                // If we get here, that means that the node's attributes (and values) all match..
                // TODO: Should we go down into the child node collections for sorting?
                //       See example `c.xml`..
                //Console.WriteLine(a.Name + "==" + b.Name + " all attributes matched");
            }

            return result;
        }

        static void SortNodeAttrs( XmlNode node )
        {
            // Remove, sort, then re-add the node's attributes.
            if (sort_attr && node.Attributes != null && node.Attributes.Count > 0) {
                SortXmlAttributeCollection(node.Attributes);
            }

            // Sort the children node's attributes also.
            for (int i = 0, len = node.ChildNodes.Count; i < len; i++) {
                SortNodeAttrs(node.ChildNodes[i]);
            }
        }

        static void SortXmlAttributeCollection( XmlAttributeCollection col )
        {
            // Remove, sort, then re-add the attributes to the collection.
            if (sort_attr && col != null && col.Count > 0) {
                var attrs = new List<XmlAttribute>(col.Count);

                for (var i = col.Count - 1; i >= 0; i--) {
                    attrs.Add(col[i]);
                    col.RemoveAt(i);
                }

                SortAttributeList(attrs);

                for (var i = 0; i < attrs.Count; i++) {
                    col.Append(attrs[i]);
                }
            }
        }

        static void SortAttributeList( List<XmlAttribute> attrs )
        {
            attrs.Sort(delegate ( XmlAttribute a, XmlAttribute b ) {
                var result = string.Compare(a.Name, b.Name, sort_attr_comp);
                if (result == 0) {
                    return string.Compare(a.Value, b.Value, sort_attr_comp);
                } else if (!string.IsNullOrEmpty(primary_attr)) {
                    // If a primary_attr is specified, it is always made the first attribute!
                    if (a.Name.Equals(primary_attr, sort_attr_comp)) {
                        return -1;
                    } else if (b.Name.Equals(primary_attr, sort_attr_comp)) {
                        return 1;
                    }
                }
                return result;
            });
        }

        static void usage()
        {
            Console.WriteLine("sortxml");
            Console.WriteLine("");
            Console.WriteLine("This is a small utility that sorts (and prettifies) xml files.");
            Console.WriteLine("It uses the Microsoft XML .NET namespace.");
            Console.WriteLine("");
            Console.WriteLine("Copyright (c) 2014-2020 Kody Brown (@wasatchwizard)");
            Console.WriteLine("");
            Console.WriteLine("    USAGE: sortxml.exe [options] infile [outfile]");
            Console.WriteLine("");
            Console.WriteLine("      infile        The name of the file to sort, etc.");
            Console.WriteLine("");
            Console.WriteLine("      outfile       The name of the file to save the output to.");
            Console.WriteLine("                    If outfile is omitted, the output is written to stdout,");
            Console.WriteLine("                    unless `--overwrite` is specified, in which case the");
            Console.WriteLine("                    output is written back to infile, overwriting it.");
            Console.WriteLine("");
            Console.WriteLine("    OPTIONS:");
            Console.WriteLine("");
            Console.WriteLine("      /p --pause    Pauses when finished.");
            Console.WriteLine("");
            Console.WriteLine("      --pretty      Ignores the input format and makes the output look nice.");
            Console.WriteLine("                    This is the default.");
            Console.WriteLine("");
            Console.WriteLine("      /s --sort     Sort both the nodes and attributes.");
            Console.WriteLine("      --sort-node   Sort the nodes.");
            Console.WriteLine("      --sort-attr   Sort the attributes.");
            Console.WriteLine("                    If a sort is specified, '--pretty' is assumed.");
            Console.WriteLine("                    If a sort is NOT is specified, both nodes and attributes");
            Console.WriteLine("                    will be sorted.");
            Console.WriteLine("");
            Console.WriteLine("      /i --case-insensitive");
            Console.WriteLine("                    Sorts node and attributes without regard to letter case.");
            Console.WriteLine("      !i --case-sensitive");
            Console.WriteLine("                    Sorts node and attributes case-sensitively.");
            Console.WriteLine("                    If neither option is specified, uses case-sensitive sort.");
            Console.WriteLine("");
            Console.WriteLine("      --overwrite   Writes back to the infile.");
            Console.WriteLine("                    Only used if outfile is not specified.");
            Console.WriteLine("");
            Console.WriteLine("    Prefix an option with '!' to turn it off.");
            Console.WriteLine("    The '!' can be applied with or without one of the other prefixes.");
            Console.WriteLine("    The '/' and '--' prefixes are interchangable.");
            Console.WriteLine("");
            Console.WriteLine("The default is to output pretty and sorted nodes and attributes:");
            Console.WriteLine("");
            Console.WriteLine("    > type sample.xml");
            Console.WriteLine("    <?xml version=\"1.0\" encoding=\"utf-8\" ?><root><node value=\"one\" attr=\"name\"/><node2 attr=\"name\" value=\"two\" /></root>");
            Console.WriteLine("");
            Console.WriteLine("    > sortxml.exe sample.xml");
            Console.WriteLine("    <?xml version=\"1.0\" encoding=\"utf-8\" ?>");
            Console.WriteLine("    <root>");
            Console.WriteLine("        <node attr=\"name\" value=\"one\" />");
            Console.WriteLine("        <node2 attr=\"name\" value=\"two\" />");
            Console.WriteLine("    </root>");

        }
    }
}
