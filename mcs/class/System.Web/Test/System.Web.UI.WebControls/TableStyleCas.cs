//
// TableStyleCas.cs 
//	- CAS unit tests for System.Web.UI.WebControls.TableStyle
//
// Author:
//	Sebastien Pouliot  <sebastien@ximian.com>
//
// Copyright (C) 2005 Novell, Inc (http://www.novell.com)
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

using NUnit.Framework;

using System;
using System.Security.Permissions;
using System.Web;
using System.Web.UI.WebControls;

using MonoTests.System.Web.UI.WebControls;

namespace MonoCasTests.System.Web.UI.WebControls {

	[TestFixture]
	[Category ("CAS")]
	public class TableStyleCas : AspNetHostingMinimal {

		[Test]
		[PermissionSet (SecurityAction.Deny, Unrestricted = true)]
		public void Deny_Unrestricted ()
		{
			TableStyleTest unit = new TableStyleTest ();
			unit.Constructor_Default ();
			unit.Constructor_StateBag_Null ();
			unit.Constructor_StateBag ();
			unit.AddAttributesToRender_Null_WebControl ();
			unit.AddAttributesToRender_HtmlTextWriter_Null ();
			unit.AddAttributesToRender ();
			unit.CopyFrom_Null ();
			unit.CopyFrom_Self ();
			unit.CopyFrom_Empty ();
			unit.CopyFrom_IsEmpty ();
			unit.CopyFrom ();
			unit.MergeWith_Null ();
			unit.MergeWith_Self ();
			unit.MergeWith_Empty ();
			unit.MergeWith ();
			unit.GridLines_VS ();
			unit.FillStyleAttributes_Null_Resolver ();
			unit.FillStyleAttributes_Css_Null ();
			unit.FillStyleAttributes_Empty ();
			unit.FillStyleAttributes_NotCss ();
			unit.FillStyleAttributes_Css_WithoutResolution ();
			unit.FillStyleAttributes_Css_WithResolution ();
		}

		// LinkDemand

		public override Type Type {
			get { return typeof (TableStyle); }
		}
	}
}
