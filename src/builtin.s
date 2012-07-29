	global _extensions_js
_extensions_js	dw		_extensions_js_end-$
				incbin 'builtin/extensions.js'
_extensions_js_end

	global _include_js
_include_js		dw		_include_js_end-$
				incbin	'builtin/include.js'
_include_js_end

	global	_print_r_js
_print_r_js		dw		_print_r_js_end-$
				incbin	'builtin/print_r.js'
_print_r_js_end

	global	_require_js
_require_js		dw		_require_js_end-$
				incbin	'builtin/require.js'
_require_js_end

