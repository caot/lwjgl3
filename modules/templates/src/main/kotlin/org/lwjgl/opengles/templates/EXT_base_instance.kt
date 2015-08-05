/*
 * Copyright LWJGL. All rights reserved.
 * License terms: http://lwjgl.org/license.php
 */
package org.lwjgl.opengles.templates

import org.lwjgl.generator.*
import org.lwjgl.opengles.*
import org.lwjgl.opengles.BufferType.*

val EXT_base_instance = "EXTBaseInstance".nativeClassGLES("EXT_base_instance", postfix = EXT) {
	nativeImport (
		"OpenGLES.h"
	)

	documentation =
		"""
		Native bindings to the $registryLink extension.

		This extension allows the offset within buffer objects used for instanced rendering to be specified. This is congruent with the <first> parameter in
		glDrawArrays and the <basevertex> parameter in glDrawElements. When instanced rendering is performed (for example, through glDrawArraysInstanced),
		instanced vertex attributes whose vertex attribute divisors are non-zero are fetched from enabled vertex arrays per-instance rather than per-vertex.
		However, in unextended OpenGL ES, there is no way to define the offset into those arrays from which the attributes are fetched. This extension adds
		that offset in the form of a <baseinstance> parameter to several new procedures.

		The <baseinstance> parameter is added to the index of the array element, after division by the vertex attribute divisor. This allows several sets of
		instanced vertex attribute data to be stored in a single vertex array, and the base offset of that data to be specified for each draw. Further, this
		extension exposes the <baseinstance> parameter as the final and previously undefined structure member of the draw-indirect data structure.

		Requires ${GLES30.core}.
		"""

	void(
		"DrawArraysInstancedBaseInstanceEXT",
		"",

		GLenum.IN("mode", ""),
		GLint.IN("first", ""),
		GLsizei.IN("count", ""),
		GLsizei.IN("instancecount", ""),
		GLuint.IN("baseinstance", "")
	)

	void(
		"DrawElementsInstancedBaseInstanceEXT",
		"",

		GLenum.IN("mode", ""),
		AutoSize("indices") shr "GLESChecks.typeToByteShift(type)" _ GLsizei.IN("count", ""),
		AutoType("indices", GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT) _ GLenum.IN("type", ""),
		ELEMENT_ARRAY_BUFFER _ const _ void_p.IN("indices", ""),
		GLsizei.IN("instancecount", ""),
		GLuint.IN("baseinstance", "")
	)

	void(
		"DrawElementsInstancedBaseVertexBaseInstanceEXT",
		"",

		GLenum.IN("mode", ""),
		AutoSize("indices") shr "GLESChecks.typeToByteShift(type)" _ GLsizei.IN("count", ""),
		AutoType("indices", GL_UNSIGNED_BYTE, GL_UNSIGNED_SHORT, GL_UNSIGNED_INT) _ GLenum.IN("type", ""),
		ELEMENT_ARRAY_BUFFER _ const _ void_p.IN("indices", ""),
		GLsizei.IN("instancecount", ""),
		GLint.IN("basevertex", ""),
		GLuint.IN("baseinstance", "")
	)
}