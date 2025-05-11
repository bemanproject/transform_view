import React from 'react';

export default function Stdref({ref}) {
    return (
        <a href={"https://eel.is/c++draft/" + ref}>[{ref}]</a>
    );
}
